// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineSweeperPluginCommands.h"

#define LOCTEXT_NAMESPACE "FMineSweeperPluginModule"

void FMineSweeperPluginCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "MineSweeperPlugin", "Execute MineSweeperPlugin action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
