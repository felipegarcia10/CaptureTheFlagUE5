// Copyright VFS 2026 - Rohit


#include "NetworkingPlayerController.h"
#include "NetworkingCameraManager.h"

ANetworkingPlayerController::ANetworkingPlayerController()
{
	PlayerCameraManagerClass = ANetworkingCameraManager::StaticClass();
}

void ANetworkingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	// The main menu / lobby PlayerController uses FInputModeUIOnly, which sets
	// GameViewportClient::bIgnoreInput = true. That flag lives on the viewport
	// client and survives ClientTravel, so without resetting it here the player
	// lands in the game with input blocked.
	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	bShowMouseCursor = false;
}
