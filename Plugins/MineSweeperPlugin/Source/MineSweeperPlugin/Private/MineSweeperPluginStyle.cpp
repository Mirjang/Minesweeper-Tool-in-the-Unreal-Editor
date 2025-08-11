// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineSweeperPluginStyle.h"
#include "MineSweeperPlugin.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMineSweeperPluginStyle::StyleInstance = nullptr;

void FMineSweeperPluginStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMineSweeperPluginStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMineSweeperPluginStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MineSweeperPluginStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FMineSweeperPluginStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MineSweeperPluginStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MineSweeperPlugin")->GetBaseDir() / TEXT("Resources"));

	Style->Set("MineSweeperPlugin.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FMineSweeperPluginStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMineSweeperPluginStyle::Get()
{
	return *StyleInstance;
}
