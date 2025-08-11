// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineSweeperPlugin.h"
#include "MineSweeperPluginStyle.h"
#include "MineSweeperPluginCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

#include "MineSweeperWidget.h"

static const FName MineSweeperPluginTabName("MineSweeperPlugin");

#define LOCTEXT_NAMESPACE "FMineSweeperPluginModule"

void FMineSweeperPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMineSweeperPluginStyle::Initialize();
	FMineSweeperPluginStyle::ReloadTextures();

	FMineSweeperPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMineSweeperPluginCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMineSweeperPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMineSweeperPluginModule::RegisterMenus));
}

void FMineSweeperPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMineSweeperPluginStyle::Shutdown();

	FMineSweeperPluginCommands::Unregister();
}

void FMineSweeperPluginModule::PluginButtonClicked()
{
	TSharedRef<SWindow> MyWindow = SNew(SWindow)
		.Title(FText::FromString(TEXT("Mine Sweeper Tool")))
		.ClientSize(FVector2D(600, 400))
		[
			SNew(SMineSweeperTool)
		];

	FSlateApplication::Get().AddWindow(MyWindow);
}

void FMineSweeperPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMineSweeperPluginCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMineSweeperPluginCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMineSweeperPluginModule, MineSweeperPlugin)
