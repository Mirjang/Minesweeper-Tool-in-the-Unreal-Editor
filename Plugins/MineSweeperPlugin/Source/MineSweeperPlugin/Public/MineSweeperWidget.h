

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SSpinBox.h"

#define DEFAULT_GRID_SIZE 5
#define DEFAULT_MINE_COUNT 4
#define CELL_SIZE 15

TArray<int32> GenerateUniqueRandomInts(int32 Count, int32 Min = 0, int32 Max = 100); //@TODO: move this into some util library somewhere

/**
 *  The actual field where we play MS
 */
class MINESWEEPERPLUGIN_API SMineSweeperWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMineSweeperWidget)
		:_GridWidth(DEFAULT_GRID_SIZE),
		_GridHeight(DEFAULT_GRID_SIZE),
		_NumMines(DEFAULT_MINE_COUNT)
	{}
		SLATE_ARGUMENT(int, GridWidth)
		SLATE_ARGUMENT(int, GridHeight)
		SLATE_ARGUMENT(int, NumMines)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);



	FReply OnGridButtonPressed(const int Column, const int Row);


private:

	void RedrawPanel();

	void RevealCell(const int Column, const int Row); 
	int32 CountMines(const int Columt, const int Row) const;
	inline int32 LinearIndex(const int Column, const int Row) const { return GridWidth * Row + Column; }


	// Internal copies of the attributes
	int GridWidth;
	int GridHeight;
	int NumMines;

	TSharedPtr<SVerticalBox> Layout;
	TSharedPtr<SUniformGridPanel> GridLayout; 

	TSet<FIntVector2> Mines; 
	TBitArray<> Revealed; 
	bool bGameOver = false; 
};


/**
 * The entire tool
 */
class MINESWEEPERPLUGIN_API SMineSweeperTool : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMineSweeperWidget)
		{
		}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);


	void StartGame(); 

private:

	TSharedPtr<SVerticalBox> Layout;

	TSharedPtr<SSpinBox<int32>> WidthInput;
	TSharedPtr<SSpinBox<int32>> HeightInput;
	TSharedPtr<SSpinBox<int32>> NumMinesInput;
	
	TSharedPtr<SMineSweeperWidget> GameWidget; 
};

