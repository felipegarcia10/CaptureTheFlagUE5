// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DemoGameInstance.h"
#include "MainMenuWidget.generated.h"

class UButton;

UCLASS()
class PG29UNREALPROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Menu")
	FString ServerName = TEXT("My Server");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Menu")
	int32 MaxPlayers = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Menu")
	FString LobbyMapPath = TEXT("/Game/Assets/Maps/LVL_Lobby");

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> BtnHost;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> BtnJoin;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> BtnQuit;

	UFUNCTION() void HandleHostClicked();
	UFUNCTION() void HandleJoinClicked();
	UFUNCTION() void HandleQuitClicked();
	UFUNCTION() void HandleSessionsFound(const TArray<FSessionListEntry>& Results);

private:
	UDemoGameInstance* GetDemoGameInstance() const;
};
