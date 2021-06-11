// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BlockyWaste : ModuleRules
{
	public BlockyWaste(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
