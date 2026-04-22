// Copyright VFS 2026 - Rohit


#include "DemoGameInstance.h"

#include "Engine.h"
#include "Logging/StructuredLog.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

DEFINE_LOG_CATEGORY(LogDemoGameInstance);

const FName UDemoGameInstance::NetSessionName = FName("CTFSession");
const FName UDemoGameInstance::ServerNameKey = FName("SERVER_NAME");

void UDemoGameInstance::Init()
{
	Super::Init();
	if (UWorld* World = GetWorld())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Game Instance Init"));
		UE_LOGFMT(LogCore, Warning, "Game Instance Initialized");
	}
}

void UDemoGameInstance::Shutdown()
{
	if (IOnlineSessionPtr Sessions = GetSessions())
	{
		Sessions->ClearOnCreateSessionCompleteDelegates(this);
		Sessions->ClearOnFindSessionsCompleteDelegates(this);
		Sessions->ClearOnJoinSessionCompleteDelegates(this);
		Sessions->ClearOnDestroySessionCompleteDelegates(this);
		Sessions->ClearOnStartSessionCompleteDelegates(this);
	}
	if (UWorld* World = GetWorld())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Game Instance Shutdown"));
		UE_LOGFMT(LogCore, Warning, "Game Instance Shutdown");
	}
	Super::Shutdown();
}

IOnlineSessionPtr UDemoGameInstance::GetSessions() const
{
	IOnlineSubsystem* Sub = IOnlineSubsystem::Get();
	return Sub ? Sub->GetSessionInterface() : nullptr;
}

bool UDemoGameInstance::IsNullSubsystem() const
{
	IOnlineSubsystem* Sub = IOnlineSubsystem::Get();
	return Sub && Sub->GetSubsystemName() == FName("NULL");
}

void UDemoGameInstance::HostSession(const FString& ServerName, int32 MaxPlayers, const FString& LobbyMapName)
{
	IOnlineSessionPtr Sessions = GetSessions();
	if (!Sessions.IsValid())
	{
		UE_LOG(LogDemoGameInstance, Warning, TEXT("HostSession: no session interface available"));
		OnHostSessionComplete.Broadcast(false);
		return;
	}

	PendingServerName = ServerName;
	PendingLobbyMap = LobbyMapName;
	PendingMaxPlayers = FMath::Max(1, MaxPlayers);

	if (Sessions->GetNamedSession(NetSessionName))
	{
		bPendingHostAfterDestroy = true;
		Sessions->OnDestroySessionCompleteDelegates.AddUObject(this, &UDemoGameInstance::HandleDestroySessionComplete);
		Sessions->DestroySession(NetSessionName);
		return;
	}

	FOnlineSessionSettings Settings;
	Settings.NumPublicConnections = PendingMaxPlayers;
	Settings.NumPrivateConnections = 0;
	Settings.bAllowJoinInProgress = true;
	Settings.bIsLANMatch = IsNullSubsystem();
	Settings.bShouldAdvertise = true;
	Settings.bUsesPresence = !IsNullSubsystem();
	Settings.bAllowJoinViaPresence = !IsNullSubsystem();
	Settings.bUseLobbiesIfAvailable = !IsNullSubsystem();
	Settings.Set(ServerNameKey, ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	Sessions->OnCreateSessionCompleteDelegates.AddUObject(this, &UDemoGameInstance::HandleCreateSessionComplete);
	Sessions->CreateSession(0, NetSessionName, Settings);
}

void UDemoGameInstance::HandleCreateSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	if (IOnlineSessionPtr Sessions = GetSessions())
	{
		Sessions->ClearOnCreateSessionCompleteDelegates(this);
	}

	OnHostSessionComplete.Broadcast(bWasSuccessful);

	if (bWasSuccessful && !PendingLobbyMap.IsEmpty())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*PendingLobbyMap), true, TEXT("listen"));
	}
}

void UDemoGameInstance::FindSessions()
{
	IOnlineSessionPtr Sessions = GetSessions();
	if (!Sessions.IsValid())
	{
		OnSessionsFound.Broadcast(TArray<FSessionListEntry>());
		return;
	}

	SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearch->MaxSearchResults = 50;
	SessionSearch->bIsLanQuery = IsNullSubsystem();
	// Note: SEARCH_PRESENCE is intentionally not set. It's only meaningful for
	// presence-capable subsystems (Steam/EOS) and its FName constant isn't
	// consistently exported across UE 5.6 builds. For LAN (Null) it's irrelevant.

	Sessions->OnFindSessionsCompleteDelegates.AddUObject(this, &UDemoGameInstance::HandleFindSessionsComplete);
	Sessions->FindSessions(0, SessionSearch.ToSharedRef());
}

void UDemoGameInstance::HandleFindSessionsComplete(bool bWasSuccessful)
{
	if (IOnlineSessionPtr Sessions = GetSessions())
	{
		Sessions->ClearOnFindSessionsCompleteDelegates(this);
	}

	TArray<FSessionListEntry> Entries;
	if (bWasSuccessful && SessionSearch.IsValid())
	{
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); ++i)
		{
			const FOnlineSessionSearchResult& R = SessionSearch->SearchResults[i];
			FSessionListEntry E;
			E.Index = i;
			E.Ping = R.PingInMs;
			E.MaxPlayers = R.Session.SessionSettings.NumPublicConnections;
			E.CurrentPlayers = FMath::Max(0, E.MaxPlayers - R.Session.NumOpenPublicConnections);
			R.Session.SessionSettings.Get(ServerNameKey, E.ServerName);
			if (E.ServerName.IsEmpty())
			{
				E.ServerName = R.Session.OwningUserName;
			}
			Entries.Add(E);
		}
	}
	OnSessionsFound.Broadcast(Entries);
}

void UDemoGameInstance::JoinSessionAtIndex(int32 Index)
{
	IOnlineSessionPtr Sessions = GetSessions();
	if (!Sessions.IsValid() || !SessionSearch.IsValid() || !SessionSearch->SearchResults.IsValidIndex(Index))
	{
		OnJoinSessionComplete.Broadcast(false);
		return;
	}

	Sessions->OnJoinSessionCompleteDelegates.AddUObject(this, &UDemoGameInstance::HandleJoinSessionComplete);
	Sessions->JoinSession(0, NetSessionName, SessionSearch->SearchResults[Index]);
}

void UDemoGameInstance::HandleJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSessionPtr Sessions = GetSessions();
	if (!Sessions.IsValid())
	{
		OnJoinSessionComplete.Broadcast(false);
		return;
	}
	Sessions->ClearOnJoinSessionCompleteDelegates(this);

	const bool bSuccess = (Result == EOnJoinSessionCompleteResult::Success);
	OnJoinSessionComplete.Broadcast(bSuccess);

	if (bSuccess)
	{
		FString ConnectString;
		if (Sessions->GetResolvedConnectString(NetSessionName, ConnectString))
		{
			if (APlayerController* PC = GetFirstLocalPlayerController())
			{
				PC->ClientTravel(ConnectString, TRAVEL_Absolute);
			}
		}
	}
}

void UDemoGameInstance::DestroyCurrentSession()
{
	IOnlineSessionPtr Sessions = GetSessions();
	if (!Sessions.IsValid() || !Sessions->GetNamedSession(NetSessionName))
	{
		return;
	}
	bPendingHostAfterDestroy = false;
	Sessions->OnDestroySessionCompleteDelegates.AddUObject(this, &UDemoGameInstance::HandleDestroySessionComplete);
	Sessions->DestroySession(NetSessionName);
}

void UDemoGameInstance::HandleDestroySessionComplete(FName InSessionName, bool bWasSuccessful)
{
	if (IOnlineSessionPtr Sessions = GetSessions())
	{
		Sessions->ClearOnDestroySessionCompleteDelegates(this);
	}

	if (bPendingHostAfterDestroy)
	{
		bPendingHostAfterDestroy = false;
		const FString Server = PendingServerName;
		const FString Map = PendingLobbyMap;
		const int32 Max = PendingMaxPlayers;
		HostSession(Server, Max, Map);
	}
}

void UDemoGameInstance::StartMatchAndTravel(const FString& GameMapName)
{
	PendingGameMap = GameMapName;

	IOnlineSessionPtr Sessions = GetSessions();
	if (Sessions.IsValid() && Sessions->GetNamedSession(NetSessionName))
	{
		Sessions->OnStartSessionCompleteDelegates.AddUObject(this, &UDemoGameInstance::HandleStartSessionComplete);
		Sessions->StartSession(NetSessionName);
		return;
	}

	if (UWorld* W = GetWorld())
	{
		W->ServerTravel(GameMapName, true);
	}
}

void UDemoGameInstance::HandleStartSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	if (IOnlineSessionPtr Sessions = GetSessions())
	{
		Sessions->ClearOnStartSessionCompleteDelegates(this);
	}
	if (UWorld* W = GetWorld())
	{
		W->ServerTravel(PendingGameMap, true);
	}
}

void UDemoGameInstance::OpenMainMenuMap(const FString& MainMenuMapName)
{
	DestroyCurrentSession();
	UGameplayStatics::OpenLevel(GetWorld(), FName(*MainMenuMapName));
}
