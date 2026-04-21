// Copyright VFS 2026 - Rohit

#include "LobbyWidget.h"
#include "Components/Button.h"
#include "DemoGameInstance.h"
#include "LobbyGameMode.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BtnStart) BtnStart->OnClicked.AddUniqueDynamic(this, &ULobbyWidget::HandleStartClicked);
	if (BtnLeave) BtnLeave->OnClicked.AddUniqueDynamic(this, &ULobbyWidget::HandleLeaveClicked);

	// Hide the Start button for non-host clients.
	if (BtnStart)
	{
		APlayerController* PC = GetOwningPlayer();
		if (!PC || !PC->HasAuthority())
		{
			BtnStart->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ULobbyWidget::HandleStartClicked()
{
	if (UWorld* W = GetWorld())
	{
		if (ALobbyGameMode* GM = Cast<ALobbyGameMode>(W->GetAuthGameMode()))
		{
			GM->RequestStartMatch();
		}
	}
}

void ULobbyWidget::HandleLeaveClicked()
{
	if (UWorld* W = GetWorld())
	{
		if (UDemoGameInstance* GI = W->GetGameInstance<UDemoGameInstance>())
		{
			GI->OpenMainMenuMap(MainMenuMapPath);
		}
	}
}
