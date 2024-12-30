// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EMDR : ModuleRules
{
	public EMDR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AugmentedReality", "LiveLinkInterface", "RenderCore", "ProceduralMeshComponent", "EyeTracker", "UXTools"});

		//PublicIncludePaths.AddRange(new string[] { "Engine/Plugins/Marketplace/UXTools/Source/UXTools/Public" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		/*PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AugmentedReality", "LiveLinkInterface", "RenderCore", "ProceduralMeshComponent", "EyeTracker" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG", "RHI" });

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}*/

		// DirectXMath is not present by default in non-Windows platforms and doesn't build in Android 
		// when using gcc so we add a definition to disable code using it to avoid build breaks.
		// See Task 218: Investigate building DirectXMath for Android and iOS.
		/*if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.HoloLens)
		{
			PrivateDefinitions.Add("UXT_DIRECTXMATH_SUPPORTED=1");
		}
		else
		{
			PrivateDefinitions.Add("UXT_DIRECTXMATH_SUPPORTED=0");
		}*/
		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
