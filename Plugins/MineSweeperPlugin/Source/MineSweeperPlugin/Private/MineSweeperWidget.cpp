


#include "MineSweeperWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMineSweeperWidget::Construct(const FArguments& InArgs)
{
	GridWidth = InArgs._GridWidth; 
	GridHeight = InArgs._GridHeight; 
	const int NumCells = GridHeight * GridWidth;
	Revealed.Init(false, NumCells);

	NumMines = FMath::Min(InArgs._NumMines, NumCells);
	const float AspectRatio = 1.0f * GridWidth / GridHeight;
	ChildSlot
	[
		SAssignNew(Layout, SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox).MinAspectRatio(AspectRatio).MaxAspectRatio(AspectRatio)
					[
						SAssignNew(GridLayout, SUniformGridPanel)
					]
			]
	];
	UE_LOG(LogTemp, Log, TEXT("Spawning MineSweeper Widget with W:%d, H: %d, Mines: %d"), GridWidth, GridHeight, NumMines);

	const TArray<int> MineIndices = GenerateUniqueRandomInts(NumMines, 0, NumCells);
	Algo::Transform(MineIndices, Mines, [&](int Index) {return FIntVector2(Index % GridWidth, Index / GridWidth); });

	RedrawPanel(); 
}

void SMineSweeperWidget::RedrawPanel()
{
	GridLayout->ClearChildren(); 

	for (int Row = 0; Row < GridHeight; ++Row)
	{
		for (int Column = 0; Column < GridWidth; ++Column)
		{
			const int CellIndex = LinearIndex(Column, Row); 
			if (Revealed[CellIndex])
			{
				const int MineCount = CountMines(Column, Row); 
				auto Text = SNew(STextBlock)
					.Text(FText::FromString(MineCount ? FString::FromInt(MineCount) : " ")) 
					.ColorAndOpacity(FSlateColor(FLinearColor::White));

				if (Mines.Contains(FIntVector2(Column, Row)))
				{
					Text->SetText(FText::FromString("x")); 
					Text->SetColorAndOpacity(FSlateColor(FColor::Red)); 
				}
				GridLayout->AddSlot(Column, Row).AttachWidget(Text);
			}
			else //not revealed
			{
				GridLayout->AddSlot(Column, Row).AttachWidget(SNew(SButton)
					.Text(FText::FromString("FUCK"))
					.ButtonColorAndOpacity(FSlateColor(FLinearColor::White))
					.OnClicked_Lambda([&](const int Column, const int Row) { return OnGridButtonPressed(Column, Row); }, Column, Row)
				);
			}
		}
	}
}

FReply SMineSweeperWidget::OnGridButtonPressed(const int Column, const int Row)
{
	if (bGameOver)
	{
		return FReply::Unhandled();
	}

	RevealCell(Column, Row);
	RedrawPanel();

	if (Mines.Contains(FIntVector2(Column, Row))) // We hit a mine -> GameOver
	{
		UE_LOG(LogTemp, Log, TEXT("MineSweeper GameOver"));
		bGameOver = true;
		Layout->AddSlot()
			[
				SNew(STextBlock)
					.Text(FText::FromString("Game Over"))
					.ColorAndOpacity(FSlateColor(FColor::Red))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 25))
			];
	}
	else if (Revealed.CountSetBits() == Revealed.Num() - NumMines)
	{
		UE_LOG(LogTemp, Log, TEXT("MineSweeper Win"));
		bGameOver = true;
		Layout->AddSlot()
			[
				SNew(STextBlock)
					.Text(FText::FromString("Win"))
					.ColorAndOpacity(FSlateColor(FColor::Green))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 25))
			];
	}

	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

int32 SMineSweeperWidget::CountMines(const int Column, const int Row) const
{
	const int Index = LinearIndex(Column, Row); 
	int Count = 0; 
	for (int r = -1; r != 2; ++r) //-1,0,1 
	{
		for (int c = -1; c != 2; ++c) //-1,0,1 
		{
			Count += Mines.Contains(FIntVector2(Column + c, Row + r)); 
		}
	}
	return Count; 
}

void SMineSweeperWidget::RevealCell(const int Column, const int Row)
{
	if (Row < 0 || Row >= GridHeight || Column < 0 || Column >= GridWidth)
	{
		return; 
	}
	const int CellIndex = LinearIndex(Column, Row);
	if (Revealed[CellIndex])
	{
		return; 
	}

	Revealed[CellIndex] = true;
	UE_LOG(LogTemp, Log, TEXT("MineSweeper: Revealed %d/%d - Mines: %d"), Column, Row, CountMines(Column, Row));

	if (!CountMines(Column, Row))//no neighbors are mines -> we can reveal all neighbors
	{

		//...(*) but when skipping the middle this feels cleaner imo
		RevealCell(Column - 1, Row - 1);
		RevealCell(Column - 1, Row);
		RevealCell(Column - 1, Row + 1);

		RevealCell(Column, Row - 1);
		RevealCell(Column, Row + 1);

		RevealCell(Column + 1, Row - 1);
		RevealCell(Column + 1, Row);
		RevealCell(Column + 1, Row + 1);
	}
}

TArray<int32> GenerateUniqueRandomInts(int32 Count, int32 Min, int32 Max)
{
	int32 Range = Max - Min + 1;
	checkf(Count <= Range, TEXT("Count cannot be greater than the size of the range"));

	// Create a list of all possible values in the range
	TArray<int32> Candidates;
	Candidates.Reserve(Range);
	for (int32 i = Min; i <= Max; ++i)
	{
		Candidates.Add(i);
	}

	// Shuffle the array using Fisher-Yates shuffle
	for (int32 i = Range - 1; i > 0; --i)
	{
		int32 SwapIndex = FMath::RandRange(0, i);
		Candidates.Swap(i, SwapIndex);
	}

	// Return the first Count elements
	return TArray<int32>(Candidates.GetData(), Count);
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMineSweeperTool::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(Layout, SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
					.Text(FText::FromString("Width"))
			]
			+ SHorizontalBox::Slot()
			[
				SAssignNew(WidthInput, SSpinBox<int32>)
					.MinValue(1)
					.Value(DEFAULT_GRID_SIZE)
			]
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
					.Text(FText::FromString("Height"))
			]
			+ SHorizontalBox::Slot()
			[
				SAssignNew(HeightInput, SSpinBox<int32>)
					.MinValue(1)
					.Value(DEFAULT_GRID_SIZE)
			]
		]

		+ SVerticalBox::Slot()
			.AutoHeight()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock)
						.Text(FText::FromString("Number Of Mines: "))
				]
				+ SHorizontalBox::Slot()
				[
					SAssignNew(NumMinesInput, SSpinBox<int32>)
						.MinValue(0)
						.Value(DEFAULT_MINE_COUNT)
				]
		]

		+ SVerticalBox::Slot()
			.AutoHeight()
		[ 
			SNew(SButton)
				.Text(FText::FromString("Start Game"))
				.OnClicked_Lambda([&]() 
				{
					StartGame(); 
					return FReply::Handled();
				})
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
			[
				SAssignNew(GameWidget, SMineSweeperWidget)
					.GridWidth(WidthInput->GetValueAttribute().Get())
					.GridHeight(HeightInput->GetValueAttribute().Get())
					.NumMines(NumMinesInput->GetValueAttribute().Get())
			]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SMineSweeperTool::StartGame()
{
	const int Slot = Layout->RemoveSlot(GameWidget.ToSharedRef()); 
	Layout->InsertSlot(Slot)
		[
			SAssignNew(GameWidget, SMineSweeperWidget)
				.GridWidth(WidthInput->GetValueAttribute().Get())
				.GridHeight(HeightInput->GetValueAttribute().Get())
				.NumMines(NumMinesInput->GetValueAttribute().Get())
		]; 
}
