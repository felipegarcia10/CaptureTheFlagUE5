// Copyright VFS 2026 - Rohit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UIPlayerController.generated.h"

class UUserWidget;

UCLASS()
class PG29UNREALPROJECT_API AUIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUIPlayerController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	bool bUIOnlyInput = true;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> ActiveWidget;
};
