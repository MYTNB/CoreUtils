// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreUtils.h"

#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "FCoreUtilsModule"

EAssetTypeCategories::Type FCoreUtilsModule::ProjectAssetCategory = static_cast<EAssetTypeCategories::Type>(0);

void FCoreUtilsModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	if (ProjectAssetCategory == EAssetTypeCategories::None)
	{
		const FText AssetCategoryDisplayName = LOCTEXT("ProjectAssetCategory", "Project Asset");
		ProjectAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Project Asset")), AssetCategoryDisplayName);
	}
}

void FCoreUtilsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCoreUtilsModule, CoreUtils)