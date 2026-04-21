// Copyright VFS 2026 - Rohit


#include "NetworkingPlayerController.h"
#include "NetworkingCameraManager.h"
#include "NetworkingGameMode.h"
#include "Kismet/GameplayStatics.h"

ANetworkingPlayerController::ANetworkingPlayerController()
{
	PlayerCameraManagerClass = ANetworkingCameraManager::StaticClass();


}

//void ANetworkingPlayerController::ClientShowGameOver_Implementation()
//{
//    CreateGameOverWidget();
//}


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

