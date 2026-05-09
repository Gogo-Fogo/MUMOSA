// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MumosaSpatial : ModuleRules
{
	public MumosaSpatial(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"MumosaSpatial",
			"MumosaSpatial/Variant_Horror",
			"MumosaSpatial/Variant_Horror/UI",
			"MumosaSpatial/Variant_Shooter",
			"MumosaSpatial/Variant_Shooter/AI",
			"MumosaSpatial/Variant_Shooter/UI",
			"MumosaSpatial/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
