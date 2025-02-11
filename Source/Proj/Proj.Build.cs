// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Proj : ModuleRules
{
	public Proj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG", "AdvancedSessions" });
        PublicDependencyModuleNames.AddRange(new string[] {
        		"Slate",
				"SlateCore",
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks"
			});
}
}
