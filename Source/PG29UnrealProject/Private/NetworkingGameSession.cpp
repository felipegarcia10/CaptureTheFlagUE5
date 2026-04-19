// Copyright VFS 2026 - Rohit


#include "NetworkingGameSession.h"
#include "GameFramework/GameMode.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogNetworkingGameSession)

FString ANetworkingGameSession::ApproveLogin(const FString& Options)
{
	const int32 MaxPlayersToUse = MaxPlayers;

	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		UE_LOGFMT(LogNetworkingGameSession, Log, "Aproving Login Request | CurentPlayers = {CurrentPlayers} / MaxPlayer = {MaxPlayers}",
			("CurrentPlayers", GameMode->GetNumPlayers()),
			("MaxPlayers", MaxPlayersToUse));
	}
	
	return Super::ApproveLogin(Options);
}
