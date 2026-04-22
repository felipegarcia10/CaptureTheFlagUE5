// Copyright VFS 2026 - Rohit


#include "NetworkingGameSession.h"

ANetworkingGameSession::ANetworkingGameSession()
{
	MaxPlayers = 100;
}

FString ANetworkingGameSession::ApproveLogin(const FString& Options)
{
	return Super::ApproveLogin(Options);
}
