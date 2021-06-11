//using System;
//using System.IO;
//using UnrealBuildTool;

//public class SQLite3Library : ModuleRules
//{
//  public SQLite3Library(ReadOnlyTargetRules Target) : base(Target)
//  {
//    Type = ModuleType.External;

//    // Add any macros that need to be set
//    //PublicDefinitions.Add("WITH_MYTHIRDPARTYLIBRARY=1");

//    string sqlite3_root = Path.Combine(ModuleDirectory, "../../extern/sqlite3");

//    //Target.Architecture;

//    // Add any include paths for the plugin
//    PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));

//    // Add any import libraries or static libraries
//    PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "sqlite3.lib"));
//  }
//}