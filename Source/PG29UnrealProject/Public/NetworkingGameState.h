// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NetworkingGameState.generated.h"

/**
 * 
 */
UCLASS()
class PG29UNREALPROJECT_API ANetworkingGameState : public AGameState
{
	GENERATED_BODY()

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowGameOver();
	
};
