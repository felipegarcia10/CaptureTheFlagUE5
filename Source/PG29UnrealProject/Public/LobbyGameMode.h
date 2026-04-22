// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class PG29UNREALPROJECT_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lobby")
	FString GameMapName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lobby")
	int32 MinPlayersToStart = 2;

	UFUNCTION(BlueprintCallable, Category="Lobby")
	void RequestStartMatch();

	UFUNCTION(BlueprintPure, Category="Lobby")
	int32 GetConnectedPlayerCount() const;

protected:
	int32 ConnectedPlayers = 0;
};
