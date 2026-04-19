// Copyright VFS 2026 - Rohit


#include "NetworkingPlayerState.h"
#include "Net/UnrealNetwork.h"

void ANetworkingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetworkingPlayerState, TeamTag);
}

void ANetworkingPlayerState::SetTeamTag_Implementation(FGameplayTag NewTeamTag)
{
	TeamTag = NewTeamTag;
}

