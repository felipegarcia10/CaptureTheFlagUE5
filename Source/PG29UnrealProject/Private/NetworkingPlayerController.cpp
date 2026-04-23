// Copyright VFS 2026 - Rohit


#include "NetworkingPlayerController.h"
#include "NetworkingCameraManager.h"
#include "NetworkingGameMode.h"
//#include "Kismet/GameplayStatics.h"

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

void ANetworkingPlayerController::Server_RequestRestart_Implementation()
{
	if (!HasAuthority())
		return;
	if (!IsLocalController())
		return;

	ANetworkingGameMode* GM = GetWorld()->GetAuthGameMode<ANetworkingGameMode>();

	if (GM)
	{
		GM->RestartGame();
	}
}