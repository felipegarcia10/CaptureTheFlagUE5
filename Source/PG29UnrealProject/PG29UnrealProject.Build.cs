// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PG29UnrealProject : ModuleRules
{
	public PG29UnrealProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayTags",
			"UMG",
			"Slate",
			"SlateCore",
			"OnlineSubsystem",
			"OnlineSubsystemUtils"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
