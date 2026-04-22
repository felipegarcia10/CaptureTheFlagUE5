// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "NetworkingGameSession.generated.h"

/**
 *
 */

UCLASS()
class PG29UNREALPROJECT_API ANetworkingGameSession : public AGameSession
{
	GENERATED_BODY()

public:

	ANetworkingGameSession();

	virtual FString ApproveLogin(const FString& Options) override;

};
