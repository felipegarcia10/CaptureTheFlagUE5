// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "DemoGameInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDemoGameInstance, Log, All);

USTRUCT(BlueprintType)
struct FSessionListEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Sessions")
	FString ServerName;

	UPROPERTY(BlueprintReadOnly, Category="Sessions")
	int32 CurrentPlayers = 0;

	UPROPERTY(BlueprintReadOnly, Category="Sessions")
	int32 MaxPlayers = 0;

	UPROPERTY(BlueprintReadOnly, Category="Sessions")
	int32 Ping = 0;

	UPROPERTY(BlueprintReadOnly, Category="Sessions")
	int32 Index = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionSearchCompleteDynamic, const TArray<FSessionListEntry>&, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionActionCompleteDynamic, bool, bSuccess);

UCLASS()
class PG29UNREALPROJECT_API UDemoGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, Category="Sessions")
	void HostSession(const FString& ServerName, int32 MaxPlayers, const FString& LobbyMapName);

	UFUNCTION(BlueprintCallable, Category="Sessions")
	void FindSessions();

	UFUNCTION(BlueprintCallable, Category="Sessions")
	void JoinSessionAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category="Sessions")
	void DestroyCurrentSession();

	UFUNCTION(BlueprintCallable, Category="Sessions")
	void StartMatchAndTravel(const FString& GameMapName);

	UFUNCTION(BlueprintCallable, Category="Sessions")
	void OpenMainMenuMap(const FString& MainMenuMapName);

	UPROPERTY(BlueprintAssignable, Category="Sessions")
	FOnSessionSearchCompleteDynamic OnSessionsFound;

	UPROPERTY(BlueprintAssignable, Category="Sessions")
	FOnSessionActionCompleteDynamic OnHostSessionComplete;

	UPROPERTY(BlueprintAssignable, Category="Sessions")
	FOnSessionActionCompleteDynamic OnJoinSessionComplete;

	static const FName NetSessionName;
	static const FName ServerNameKey;

private:
	void HandleCreateSessionComplete(FName InSessionName, bool bWasSuccessful);
	void HandleFindSessionsComplete(bool bWasSuccessful);
	void HandleJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);
	void HandleDestroySessionComplete(FName InSessionName, bool bWasSuccessful);
	void HandleStartSessionComplete(FName InSessionName, bool bWasSuccessful);

	IOnlineSessionPtr GetSessions() const;
	bool IsNullSubsystem() const;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	FString PendingServerName;
	FString PendingLobbyMap;
	FString PendingGameMap;
	int32 PendingMaxPlayers = 2;
	bool bPendingHostAfterDestroy = false;
};
