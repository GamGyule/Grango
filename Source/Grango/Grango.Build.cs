// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Grango : ModuleRules
{
	public Grango(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"ProceduralMeshComponent"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]{"Slate", "SlateCore"});
	}
}
