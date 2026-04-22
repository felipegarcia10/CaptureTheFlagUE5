// Copyright VFS 2026 - Rohit

#include "UIPlayerController.h"
#include "Blueprint/UserWidget.h"

AUIPlayerController::AUIPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController() || !WidgetClass)
	{
		return;
	}

	ActiveWidget = CreateWidget<UUserWidget>(this, WidgetClass);
	if (ActiveWidget)
	{
		ActiveWidget->AddToViewport();
	}

	if (bUIOnlyInput)
	{
		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(ActiveWidget ? ActiveWidget->TakeWidget() : TSharedPtr<SWidget>());
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(Mode);
	}
	else
	{
		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(ActiveWidget ? ActiveWidget->TakeWidget() : TSharedPtr<SWidget>());
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		Mode.SetHideCursorDuringCapture(false);
		SetInputMode(Mode);
	}

	bShowMouseCursor = true;
}

void AUIPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ActiveWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}
