// Copyright VFS 2026 - Rohit

#include "LobbyGameMode.h"
#include "DemoGameInstance.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = false;
	DefaultPawnClass = ASpectatorPawn::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++ConnectedPlayers;
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	ConnectedPlayers = FMath::Max(0, ConnectedPlayers - 1);
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
		return;
	}
	if (ConnectedPlayers < MinPlayersToStart)
	{
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
