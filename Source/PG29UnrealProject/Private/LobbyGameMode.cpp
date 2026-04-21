// Copyright VFS 2026 - Rohit

#include "LobbyGameMode.h"
#include "DemoGameInstance.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogLobbyGameMode);

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = false;
	DefaultPawnClass = ASpectatorPawn::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++ConnectedPlayers;
	UE_LOG(LogLobbyGameMode, Log, TEXT("Lobby: player joined (%d connected)"), ConnectedPlayers);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	ConnectedPlayers = FMath::Max(0, ConnectedPlayers - 1);
	UE_LOG(LogLobbyGameMode, Log, TEXT("Lobby: player left (%d connected)"), ConnectedPlayers);
	Super::Logout(Exiting);
}

int32 ALobbyGameMode::GetConnectedPlayerCount() const
{
	return ConnectedPlayers;
}

void ALobbyGameMode::RequestStartMatch()
{
	if (GameMapName.IsEmpty())
	{
		UE_LOG(LogLobbyGameMode, Warning, TEXT("Lobby: GameMapName not set, cannot start match"));
		return;
	}
	if (ConnectedPlayers < MinPlayersToStart)
	{
		UE_LOG(LogLobbyGameMode, Warning, TEXT("Lobby: need %d players to start (have %d)"), MinPlayersToStart, ConnectedPlayers);
		return;
	}

	if (UDemoGameInstance* GI = GetGameInstance<UDemoGameInstance>())
	{
		GI->StartMatchAndTravel(GameMapName);
	}
	else if (UWorld* W = GetWorld())
	{
		W->ServerTravel(GameMapName, true);
	}
}
