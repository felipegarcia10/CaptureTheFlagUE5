// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UButton;

UCLASS()
class PG29UNREALPROJECT_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Lobby")
	FString MainMenuMapPath = TEXT("/Game/Assets/Maps/LVL_MainMenu");

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> BtnStart;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> BtnLeave;

	UFUNCTION() void HandleStartClicked();
	UFUNCTION() void HandleLeaveClicked();
};
