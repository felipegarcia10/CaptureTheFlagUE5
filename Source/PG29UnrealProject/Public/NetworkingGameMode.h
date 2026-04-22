// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameplayTagContainer.h"
#include "NetworkingGameMode.generated.h"

/**
 * 
 */

class ANetworkingTeamPlayerStart;

UCLASS()
class PG29UNREALPROJECT_API ANetworkingGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ANetworkingGameMode();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* ControllerToChoose) override;

	bool ReadyToEndMatch_Implementation() override;

	// Properties

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teams")
	FGameplayTag Tag_TeamA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teams")
	FGameplayTag Tag_TeamB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teams|Spawn")
	TSubclassOf<ANetworkingTeamPlayerStart> SpawnPointClass;
	
	void OnPlayerDied(AController* Controller);

private:

	int32 GetTeamCount(const FGameplayTag& TeamTag) const;
	
};
