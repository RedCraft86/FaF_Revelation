using UnrealBuildTool;

public class GameEditor : ModuleRules
{
    public GameEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "UnrealEd",
                "Engine",
                "Slate",
                "SlateCore",
                "FaF_Revelation",
                "ToroEditor",
                "ToroRuntime",
                "ToroCore",
                "FlowEditor",
                "Flow"
            ]
        );
    }
}