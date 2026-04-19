// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"
#include "NetworkingPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PG29UNREALPROJECT_API ANetworkingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Team")
	FGameplayTag TeamTag;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void SetTeamTag(FGameplayTag NewTeamTag);
	
};
