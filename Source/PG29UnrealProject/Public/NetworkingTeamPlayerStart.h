// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "GameplayTagContainer.h"
#include "NetworkingTeamPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PG29UNREALPROJECT_API ANetworkingTeamPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	FGameplayTagContainer TeamTag;
	
};
