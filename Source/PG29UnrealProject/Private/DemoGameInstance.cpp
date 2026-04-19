// Copyright VFS 2026 - Rohit


#include "DemoGameInstance.h"
#include "Logging/StructuredLog.h"
#include "Engine.h"


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
	if (UWorld* World = GetWorld())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Game Instance Shutdown"));
		UE_LOGFMT(LogCore, Warning, "Game Instance Shutdown");
	}
	Super::Shutdown();
}
