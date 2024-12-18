// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyProject4Target : TargetRules
{
	public MyProject4Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "MyProject4" } );
		
		// Update build settings
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		// Update include order version
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
	}
}
