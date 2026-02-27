// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MDamage : ModuleRules
{
	public MDamage(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"MUtility",
				"MTeam",
				"MSimpleGAS",

				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			}
		);

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(
				new[]
				{
					"UnrealEd"
				});
		}
	}
}