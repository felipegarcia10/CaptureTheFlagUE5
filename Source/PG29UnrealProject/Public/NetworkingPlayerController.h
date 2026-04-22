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

protected:
	virtual void BeginPlay() override;
};
