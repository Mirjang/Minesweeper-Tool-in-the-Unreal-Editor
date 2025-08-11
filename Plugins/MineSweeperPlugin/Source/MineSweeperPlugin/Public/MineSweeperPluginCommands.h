// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MineSweeperPluginStyle.h"

class FMineSweeperPluginCommands : public TCommands<FMineSweeperPluginCommands>
{
public:

	FMineSweeperPluginCommands()
		: TCommands<FMineSweeperPluginCommands>(TEXT("MineSweeperPlugin"), NSLOCTEXT("Contexts", "MineSweeperPlugin", "MineSweeperPlugin Plugin"), NAME_None, FMineSweeperPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
