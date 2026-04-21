// Copyright VFS 2026 - Rohit


#include "NetworkingGameState.h"
#include "NetworkingPlayerController.h"

void ANetworkingGameState::Multicast_ShowGameOver_Implementation()
{
    for (APlayerState* PS : PlayerArray)
    {
        if (PS)
        {
            ANetworkingPlayerController* PC =
                Cast<ANetworkingPlayerController>(PS->GetOwner());

            if (PC)
            {
                PC->ClientShowGameOver();
            }
        }
    }
}