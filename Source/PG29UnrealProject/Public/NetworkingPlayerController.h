// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetworkingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PG29UNREALPROJECT_API ANetworkingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ANetworkingPlayerController();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_RequestRestart();

	UFUNCTION(Client, Reliable)
	void ClientShowGameOver();
	
};
