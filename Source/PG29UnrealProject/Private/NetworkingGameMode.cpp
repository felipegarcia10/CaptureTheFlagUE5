// Copyright VFS 2026 - Rohit


#include "NetworkingGameMode.h"
#include "NetworkingGameSession.h"
#include "NetworkingGameState.h"
#include "NetworkingCharacter.h"
#include "NetworkingPlayerController.h"
#include "NetworkingPlayerState.h"
#include "NetworkingHUD.h"
#include "NetworkingTeamPlayerStart.h"
#include "Engine.h"
#include "EngineUtils.h"
#include "Logging/StructuredLog.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogNetworkingGameMode)

ANetworkingGameMode::ANetworkingGameMode()
{
	GameSessionClass = ANetworkingGameSession::StaticClass();
	GameStateClass = ANetworkingGameState::StaticClass();
	PlayerControllerClass = ANetworkingPlayerController::StaticClass();
	PlayerStateClass = ANetworkingPlayerState::StaticClass();
	HUDClass = ANetworkingHUD::StaticClass();
	DefaultPawnClass = ANetworkingCharacter::StaticClass();

	bStartPlayersAsSpectators = true;
}

void ANetworkingGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	if (!ErrorMessage.IsEmpty())
	{
		UE_LOGFMT(LogNetworkingGameMode, Log, "PreLogin| Login Failed from Address = {Address} | Reason = {Reason} | CurrentPlayers = {NumPlayers}",
			("Address", Address),
			("Reason", ErrorMessage),
			("NumPlayers", GetNumPlayers()));
	}

}

void ANetworkingGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer)
	{
		if (ANetworkingPlayerState* LoggedInPlayerState = NewPlayer->GetPlayerState<ANetworkingPlayerState>())
		{
			UE_LOGFMT(LogNetworkingGameMode, Log, "PostLogin| Player Joined : Name = {PlayerName} | ID = {PlayerID} | TotalPlayers = {NumPlayers}",
				("PlayerName", LoggedInPlayerState->GetPlayerName()),
				("PlayerID", LoggedInPlayerState->GetPlayerId()),
				("NumPlayers", GetNumPlayers()));


			const int32 CountTeamA = GetTeamCount(Tag_TeamA);
			const int32 CountTeamB = GetTeamCount(Tag_TeamB);

			LoggedInPlayerState->SetTeamTag((CountTeamA <= CountTeamB) ? Tag_TeamA : Tag_TeamB);

			if (GetNumPlayers()  >= GameSession->MaxPlayers)
			{
				StartMatch();
			}
		}
	}
}

AActor* ANetworkingGameMode::ChoosePlayerStart_Implementation(AController* ControllerToChoose)
{
	if (ControllerToChoose)
	{
		if (ANetworkingPlayerState* PlayerState = ControllerToChoose->GetPlayerState<ANetworkingPlayerState>())
		{
			TArray<ANetworkingTeamPlayerStart*> ValidPlayerStarts;

			if (UWorld* World = GetWorld())
			{
				if (SpawnPointClass)
				{
					for (TActorIterator<ANetworkingTeamPlayerStart> It(GetWorld(), SpawnPointClass); It; ++It)
					{
						ANetworkingTeamPlayerStart* StartPoint = *It;
						if (StartPoint && StartPoint->TeamTag.HasTag(PlayerState->TeamTag))
						{
							ValidPlayerStarts.Add(StartPoint);
						}
					}
				}
			}
			if (!ValidPlayerStarts.IsEmpty())
			{
				return ValidPlayerStarts[FMath::RandRange(0, ValidPlayerStarts.Num() - 1)];
			}
		}
	}
	return Super::ChoosePlayerStart_Implementation(ControllerToChoose);
}

bool ANetworkingGameMode::ReadyToEndMatch_Implementation()
{
	if (GetNumPlayers() < GameSession->MaxPlayers)
	{
		return false;
	}

	TArray<const ANetworkingPlayerState*> TeamA;
	TArray<const ANetworkingPlayerState*> TeamB;
	if  (ANetworkingGameState* NetworkingGameState = GetGameState<ANetworkingGameState>())
	{
		for (const APlayerState* PlayerStateIt : NetworkingGameState->PlayerArray)
		{
			if (const ANetworkingPlayerState* NetworkingPlayerState = Cast<ANetworkingPlayerState>(PlayerStateIt))
			{
				if (NetworkingPlayerState->TeamTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("NetworkingGame.Character.Team.A"))))
				{
					TeamA.AddUnique(NetworkingPlayerState);
				}
				else
				{
					TeamB.AddUnique(NetworkingPlayerState);
				}
			}
		}
	}

	bool IsTeamAAlive = false;
	for (const ANetworkingPlayerState* TeamAPlayerState : TeamA)
	{
		if (TeamAPlayerState->GetPawn())
		{
			IsTeamAAlive = true;
		}
	}
	bool IsTeamBAlive = false;
	for (const ANetworkingPlayerState* TeamBPlayerState : TeamB)
	{
		if (TeamBPlayerState->GetPawn())
		{
			IsTeamBAlive = true;
		}
	}

	return !(IsTeamAAlive && IsTeamBAlive);
}

void ANetworkingGameMode::OnPlayerDied(AController* Controller)
{
	if (Controller)
	{
		FTimerHandle RespawnTimerHandle;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, [this, Controller]()
		{
			RestartPlayer(Controller);
		}, 5.0f, false);
	}
}

int32 ANetworkingGameMode::GetTeamCount(const FGameplayTag& TeamTag) const
{
	int32 Count = 0;
	for (const APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (const ANetworkingPlayerState* NetPlayerState = Cast<ANetworkingPlayerState>(PlayerState))
		{
			if (NetPlayerState->TeamTag.MatchesTag(TeamTag))
			{
				++Count;
			}
		}
	}
	return Count;
}
