// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DeepCaves : ModuleRules
{
	public DeepCaves(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "ProceduralMeshComponent" });
	}
}
