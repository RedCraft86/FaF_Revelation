using UnrealBuildTool;

public class ToroEditor : ModuleRules
{
    public ToroEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = true;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "ContentBrowser",
                "ContentBrowserData",
                "EditorScriptingUtilities",
                "DeveloperSettings",
                "EditorSubsystem",
                "AssetDefinition",
                "PropertyEditor",
                "ToolMenus",
                "UnrealEd",
                "Projects",
                "ToroCore",
                "ToroUtilities",
            }
        );
    }
}