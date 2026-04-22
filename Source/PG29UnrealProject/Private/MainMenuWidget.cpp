// Copyright VFS 2026 - Rohit

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BtnHost) BtnHost->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::HandleHostClicked);
	if (BtnJoin) BtnJoin->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::HandleJoinClicked);
	if (BtnQuit) BtnQuit->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::HandleQuitClicked);

	if (UDemoGameInstance* GI = GetDemoGameInstance())
	{
		GI->OnSessionsFound.AddUniqueDynamic(this, &UMainMenuWidget::HandleSessionsFound);
	}
}

void UMainMenuWidget::NativeDestruct()
{
	if (UDemoGameInstance* GI = GetDemoGameInstance())
	{
		GI->OnSessionsFound.RemoveDynamic(this, &UMainMenuWidget::HandleSessionsFound);
	}
	Super::NativeDestruct();
}

UDemoGameInstance* UMainMenuWidget::GetDemoGameInstance() const
{
	return GetWorld() ? GetWorld()->GetGameInstance<UDemoGameInstance>() : nullptr;
}

void UMainMenuWidget::HandleHostClicked()
{
	if (UDemoGameInstance* GI = GetDemoGameInstance())
	{
		GI->HostSession(ServerName, MaxPlayers, LobbyMapPath);
	}
}

void UMainMenuWidget::HandleJoinClicked()
{
	if (UDemoGameInstance* GI = GetDemoGameInstance())
	{
		GI->FindSessions();
	}
}

void UMainMenuWidget::HandleQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UMainMenuWidget::HandleSessionsFound(const TArray<FSessionListEntry>& Results)
{
	if (Results.Num() == 0)
	{
		return;
	}
	if (UDemoGameInstance* GI = GetDemoGameInstance())
	{
		GI->JoinSessionAtIndex(0);
	}
}
