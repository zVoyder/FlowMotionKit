// Copyright VUEDK, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FlowMotion : ModuleRules
{
	public FlowMotion(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{
		});

		PrivateIncludePaths.AddRange(new string[]
		{
		});
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"Engine"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Slate",
			"SlateCore"
		});

		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
		});
	}
}
