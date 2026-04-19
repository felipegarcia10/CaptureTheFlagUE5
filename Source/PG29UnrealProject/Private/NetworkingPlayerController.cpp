// Copyright VFS 2026 - Rohit


#include "NetworkingPlayerController.h"
#include "NetworkingCameraManager.h"

ANetworkingPlayerController::ANetworkingPlayerController()
{
	PlayerCameraManagerClass = ANetworkingCameraManager::StaticClass();
}
