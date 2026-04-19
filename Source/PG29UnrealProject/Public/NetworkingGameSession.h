// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "NetworkingGameSession.generated.h"

/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(LogNetworkingGameSession, Log, All);

UCLASS()
class PG29UNREALPROJECT_API ANetworkingGameSession : public AGameSession
{
	GENERATED_BODY()

public:

	virtual FString ApproveLogin(const FString& Options) override;
	
};
