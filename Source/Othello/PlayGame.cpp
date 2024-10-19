// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGame.h"
#include "Board.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Map.h"
#include "Animation/WidgetAnimation.h"
#include "BoardPart.h"
#include "OthelloInstance.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "OthelloGameModeBase.h"
#include "Timermanager.h"
#include "NumberWidget.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Algo/Reverse.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/VerticalBox.h"

bool UPlayGame::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	Instance = Cast<UOthelloInstance>(GetWorld()->GetGameInstance());
	if (!Instance)
		return false;

	OthelloGameMode = Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!OthelloGameMode)
		return false;
	InitGameSetting();
	return true;
}

void UPlayGame::InitGameSetting()
{
	Board = Cast<UBoard>(CreateWidget(this, Board_BP));
	Board->SetOwningClass(this);
	PlayBox->AddChildToVerticalBox(Board);
	UVerticalBoxSlot* VerticalBoxSlot = PlayBox->AddChildToVerticalBox(Board);
	if (VerticalBoxSlot) {
		FSlateChildSize SlotSize;
		SlotSize.Value = 1.0f;
		SlotSize.SizeRule = ESlateSizeRule::Fill;
		VerticalBoxSlot->SetSize(SlotSize);
		VerticalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		VerticalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	}
	Turn = false;
	AnimationFinished.Clear();
	AnimationFinished.BindUFunction(this, FName(FString(TEXT("WhenPassFinished"))));
	BindToAnimationFinished(PassAnim, AnimationFinished);

	FString str = Turn ? "Player Turn" : "CPU Turn";
	PlayerTurnText->SetText(FText::FromString(str));
	UpdateScoreBox();

	ResetTimer();
	InitTimerBox();

	for (int64 i = 0; i < Instance->SizeOfBoard; i++) {
		for (int64 j = 0; j < Instance->SizeOfBoard; j++) {
			if (i == Instance->SizeOfBoard / 2 - 1 && j == Instance->SizeOfBoard / 2 - 1)
				BoardStates.Add(FPos(i, j), EBoardState::WHITE);
			else if (i == Instance->SizeOfBoard / 2 - 1 && j == Instance->SizeOfBoard / 2)
				BoardStates.Add(FPos(i, j), EBoardState::BLACK);
			else if (i == Instance->SizeOfBoard / 2 && j == Instance->SizeOfBoard / 2 - 1)
				BoardStates.Add(FPos(i, j), EBoardState::BLACK);
			else if (i == Instance->SizeOfBoard / 2 && j == Instance->SizeOfBoard / 2)
				BoardStates.Add(FPos(i, j), EBoardState::WHITE);
			else
				BoardStates.Add(FPos(i, j), EBoardState::NONE);
			AddBoardWeight(FPos(i, j));
		}
	}
}
void UPlayGame::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateSetUp();
	Board->DisableAllParts();
	AI_Execute();
}


void UPlayGame::BtnOnClicked(FPos ClickedPos)
{
	UpdateCurrentTurn(ClickedPos);
	TurnChange();
}

void UPlayGame::TurnChange()
{
	ResetTimer();
	if (!IsGameOver()) {
		ProgressNextTurn();

		if (IsPass()) {
			TurnText->SetText(FText::FromString("Pass!"));
			PlayAnimation(PassAnim);
		}
		else {
			if (!Turn) {
				Board->DisableAllParts();
				AI_Execute();
			}
		}
	}
	else {
		GameOver();
	}
}


void UPlayGame::UpdateCurrentTurn(FPos ClickedPos)
{
	UpdateReverse(ClickedPos);
	UpdateSetUp();
	UpdateScore();
}

void UPlayGame::ProgressNextTurn()
{
	Turn = !Turn;
	FString str = Turn ? "Player Turn" : "CPU Turn";
	PlayerTurnText->SetText(FText::FromString(str));

	UpdateSetUp();
}

void UPlayGame::WhenPassFinished()
{
	ProgressNextTurn();
	ResetTimer();
	if (IsPass()) {
		GameOver();
	}
	else {
		if (!Turn) {
			Board->DisableAllParts();
			AI_Execute();
		}
	}
}

void UPlayGame::GameOver()
{
	GetWorld()->GetTimerManager().ClearTimer(TurnLimitTimeHandle);
	TurnText->SetText(FText::FromString("Stop!"));
	PlayAnimation(GameOverAnim,0.0F,0);
	GetWorld()->GetTimerManager().SetTimer(GameOverDelayHandle, this, &UPlayGame::GoToGameOverScreen, 0.1f, false, 2.0f);
}

void UPlayGame::GoToGameOverScreen() {
	OthelloGameMode->ChangeMainWidget(GameOver_BP);
}

void UPlayGame::SetBoardColor(int64 PosX, int64 PosY, UBoardPart* Board_Part)
{
	if (PosX % 2 == 0) {
		if (PosY % 2 == 0) {
			Board_Part->BoardBtn->SetBackgroundColor(FLinearColor(0.0f, 0.258183f, 0.048172f));
		}
		else {
			Board_Part->BoardBtn->SetBackgroundColor(FLinearColor(0.0f, 0.114583f, 0.021379f));
		}
	}
	else {
		if (PosY % 2 == 0) {
			Board_Part->BoardBtn->SetBackgroundColor(FLinearColor(0.0f, 0.114583f, 0.021379f));
		}
		else {
			Board_Part->BoardBtn->SetBackgroundColor(FLinearColor(0.0f, 0.258183f, 0.048172f));
		}
	}
}

void UPlayGame::ManageTurnTime()
{
	if (CurrentTurnTime > 0)
		CurrentTurnTime--;
	else {
		TurnChange();
	}
	UpdateTimerBox();
}

void UPlayGame::UpdateTimerBox()
{
	TArray<int64> NumberOfDigits = GetNumberOfDigits(CurrentTurnTime);

	for (int i = 0; i < TimerNumbers.Num(); i++) {
		if (i >= NumberOfDigits.Num()) {
			TimerNumbers[i]->ChangeNumber(-1);                                    
		}
		else {
			TimerNumbers[i]->ChangeNumber(NumberOfDigits[i]);
		}
	}
}

void UPlayGame::InitTimerBox()
{
	TArray<int64> NumberOfDigits = GetNumberOfDigits(Instance->GetLimitTime());

	for (int i = 0; i <NumberOfDigits.Num(); i++) {
		class UNumberWidget* NumberImage = Cast<UNumberWidget>(CreateWidget(this, NumberWidget));

		UHorizontalBoxSlot* HorizontalBoxSlot = TimerBox->AddChildToHorizontalBox(NumberImage);
		if (HorizontalBoxSlot) {
			HorizontalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
			HorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		}

		NumberImage->InitNumber(NumberOfDigits[i]);
		NumberImage->SetColorNumberToBlack();
		TimerNumbers.Add(NumberImage);
	}
}

void UPlayGame::UpdateScoreBox()
{
	int64 P1Score = Instance->GetP1Status().Score;
	int64 P2Score = Instance->GetP2Status().Score;
	TArray<int64> NumberOfDigitsP1 = GetNumberOfDigits(P1Score);
	TArray<int64> NumberOfDigitsP2 = GetNumberOfDigits(P2Score);

	if (P1ScoreNumbers.Num() < NumberOfDigitsP1.Num()) {

		FillArray(NumberOfDigitsP1.Num() - P1ScoreNumbers.Num(), P1ScoreNumbers, P1ScoreBox, true);
	}

	for (int i = 0; i < P1ScoreNumbers.Num(); i++) {
		if (i >= NumberOfDigitsP1.Num()) {
			P1ScoreNumbers[i]->ChangeNumber(-1);
		}
		else {
			P1ScoreNumbers[i]->ChangeNumber(NumberOfDigitsP1[i]);
		}
	}

	if (P2ScoreNumbers.Num() < NumberOfDigitsP2.Num()) {

		FillArray(NumberOfDigitsP2.Num() - P2ScoreNumbers.Num(), P2ScoreNumbers, P2ScoreBox, false);
	}
	for (int i = 0; i < P2ScoreNumbers.Num(); i++) {
		if (i >= NumberOfDigitsP2.Num()) {
			P2ScoreNumbers[i]->ChangeNumber(-1);
		}
		else {
			P2ScoreNumbers[i]->ChangeNumber(NumberOfDigitsP2[i]);
		}
	}

}

void UPlayGame::ResetTimer()
{
	CurrentTurnTime = Instance->GetLimitTime() + 1;
	GetWorld()->GetTimerManager().ClearTimer(TurnLimitTimeHandle);
	GetWorld()->GetTimerManager().SetTimer(TurnLimitTimeHandle, this, &UPlayGame::ManageTurnTime, 1.0f, true, 0.0f);
}

TArray<int64> UPlayGame::GetNumberOfDigits(int64 num)
{
	TArray<int64> NumberOfDigits;
	if (num == 0)
		NumberOfDigits.Add(0);
	while (num != 0) {
		int64 result = num % 10;
		NumberOfDigits.Add(result);
		num /= 10;
	}
	Algo::Reverse(NumberOfDigits);
	return NumberOfDigits;
}

void UPlayGame::FillArray(int64 count, TArray<class UNumberWidget*>& NumberWidgetArray, class UHorizontalBox* Parent, bool NumberColor)
{
	for (int i = 0; i < count; i++) {
		class UNumberWidget* NumberImage = Cast<UNumberWidget>(CreateWidget(this, NumberWidget));
		UHorizontalBoxSlot* HorizontalBoxSlot = Parent->AddChildToHorizontalBox(NumberImage);
		if (HorizontalBoxSlot) {
			HorizontalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			HorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		}
		NumberImage->InitNumber(0);
		if (NumberColor) 
			NumberImage->SetColorNumberToBlack();
		else
			NumberImage->SetColorNumberToWhite();
		NumberWidgetArray.Add(NumberImage);
	}
}

TArray<FPos> UPlayGame::GetReversePos(FPos ClickedPos, TMap<FPos, EBoardState> BoardMap, EBoardState CurrState) {
	TArray<int64> DirX = { 0,0,1,1,1,-1,-1,-1 };
	TArray<int64> DirY = { 1,-1,0,1,-1,0,1,-1 };
	TArray<FPos> Result;
	for (int i = 0; i < DirX.Num(); i++) {
		auto TempPos = ClickedPos;
		TArray<FPos> Parts;
		while (true) {
			int64 nx = TempPos.PosX + DirX[i];
			int64 ny = TempPos.PosY + DirY[i];

			if (nx < 0 || ny < 0 || nx >= Instance->SizeOfBoard || ny >= Instance->SizeOfBoard) {
				break;
			}
			else {
				if (BoardMap[FPos(nx, ny)] == EBoardState::NONE)
					break;
				else if (BoardMap[FPos(nx, ny)] == CurrState) {
					Result.Append(Parts);
					break;
				}
				else if (BoardMap[FPos(nx, ny)] != CurrState) {
					Parts.Add(FPos(nx, ny));
					TempPos = FPos(nx, ny);
					continue;
				}
			}
		}
	}
	return Result;
}

TArray<FPos> UPlayGame::GetSetUpPos(TMap<FPos, EBoardState> BoardMap, EBoardState CurrState) {
	TArray<FPos> Result;

	for (auto Iter = BoardMap.CreateIterator(); Iter; ++Iter) {
		if (Iter->Value == EBoardState::NONE) {
			if (CanSetUp(Iter->Key, CurrState, BoardMap)) {
				Result.Add(Iter->Key);
			}
		}
	}
	return Result;
}

bool UPlayGame::CanSetUp(FPos DefaultPos, EBoardState CurrState, TMap<FPos, EBoardState> BoardMap) {
	TArray<FPos> Dir = { FPos(0,1),FPos(0,-1) ,FPos(-1,0) ,FPos(1,0) ,FPos(1,1) ,FPos(-1,-1) ,FPos(1,-1) ,FPos(-1,1) };
	for (int i = 0; i < Dir.Num(); i++) {
		FPos Pos = DefaultPos;
		bool FindDifferentState = false;
		while (true) {
			FPos NextPos = Dir[i] + Pos;
			if (NextPos.PosX < 0 || NextPos.PosY < 0 || NextPos.PosX >= Instance->SizeOfBoard || NextPos.PosY >= Instance->SizeOfBoard) {
				break;
			}
			else {
				if (BoardMap[NextPos] == EBoardState::NONE) {
					break;
				}
				else if (CurrState == BoardMap[NextPos]) {
					if (FindDifferentState)
						return true;
					else
						break;
				}
				else if (CurrState != BoardMap[NextPos]) {
					FindDifferentState = true;
					Pos = NextPos;
					continue;
				}
			}
		}
	}
	return false;
}

bool UPlayGame::IsPass() {
	TArray<FPos> PosArr = GetSetUpPos(BoardStates, Turn ? EBoardState::WHITE : EBoardState::BLACK);
	Board->UpdateSetUp(PosArr);
	return PosArr.Num() > 0 ? false : true;
}

void UPlayGame::UpdateScore() {
	int64 PlayerOneScore = 0;
	int64 PlayerTwoScore = 0;

	for (int i = 0; i < Instance->SizeOfBoard; i++) {
		for (int j = 0; j < Instance->SizeOfBoard; j++) {
			if (BoardStates[FPos(i, j)] == EBoardState::WHITE)
				PlayerOneScore++;
			else if (BoardStates[FPos(i, j)] == EBoardState::BLACK)
				PlayerTwoScore++;
		}
	}

	Instance->SetPlayerScore(PlayerOneScore, PlayerTwoScore);
	UpdateScoreBox();
}

void UPlayGame::UpdateReverse(FPos ClickedPos)
{
	BoardStates[ClickedPos] = Turn ? EBoardState::WHITE : EBoardState::BLACK;
	TArray<FPos> Parts = GetReversePos(ClickedPos,BoardStates, Turn ? EBoardState::WHITE : EBoardState::BLACK);
	for (int64 i = 0; i < Parts.Num(); i++) {
		BoardStates[Parts[i]] = Turn ? EBoardState::WHITE : EBoardState::BLACK;
	}
	Board->UpdateReverse(Parts);
}

void UPlayGame::UpdateSetUp()
{
	TArray<FPos> Parts = GetSetUpPos(BoardStates, Turn ? EBoardState::WHITE : EBoardState::BLACK);
	Board->UpdateSetUp(Parts);
}

bool UPlayGame::IsGameOver() {
	int64 PlayerOneScore = 0;
	int64 PlayerTwoScore = 0;

	for (auto Iter = BoardStates.CreateIterator(); Iter; ++Iter) {
		if (Iter->Value == EBoardState::WHITE)
			PlayerOneScore++;
		else if (Iter->Value == EBoardState::BLACK)
			PlayerTwoScore++;
	}

	if (PlayerOneScore + PlayerTwoScore == Instance->GetSizeOfBoard() * Instance->GetSizeOfBoard() || PlayerOneScore == 0 || PlayerTwoScore == 0)
		return true;

	return false;
}

void UPlayGame::AI_Execute() {
	FPos SetUpPosition = AI_CalculatePos();
	GetWorld()->GetTimerManager().SetTimer(CPUHandle, Board->BoardMap[SetUpPosition], &UBoardPart::SetUp, 1.0f, false);
}

FPos UPlayGame::AI_CalculatePos() {
	FPos Result = MiniMax(FPos(-1, -1), 0, 4, BoardStates, !Turn, INT64_MIN, INT64_MAX).Key;
	return Result;
}

TTuple<FPos, int64> UPlayGame::MiniMax(FPos ClickedPos, int64 CurrDepth, int64 MaxDepth,TMap<FPos, EBoardState> Table, bool CPUTurn, int64 Alpha, int64 Beta) {
	EBoardState CurrState = CPUTurn ? EBoardState::BLACK : EBoardState::WHITE;
	TArray<FPos> SetUpPos = GetSetUpPos(Table, CurrState);

	if (MaxDepth == 0 || CurrDepth == MaxDepth || SetUpPos.Num() == 0) {
		int64 Result = 0;
		for (auto Iter = Table.CreateIterator(); Iter; ++Iter) {
			if (Iter->Value == EBoardState::WHITE)
				Result -= BoardWeight[Iter->Key];
			else if (Iter->Value == EBoardState::BLACK)
				Result += BoardWeight[Iter->Key];
		}
		return MakeTuple(ClickedPos, Result);
	}

	if (CPUTurn) {
		TTuple<FPos, int64> Result = MakeTuple(ClickedPos, INT64_MIN);
		for (int i = 0; i < SetUpPos.Num(); i++) {
			TMap<FPos, EBoardState> NextTable = Table;
			NextTable[SetUpPos[i]] = EBoardState::BLACK;
			TArray<FPos> ReversePos = GetReversePos(SetUpPos[i], NextTable, CurrState);
			for (int j = 0; j < ReversePos.Num(); j++) {
				NextTable[ReversePos[j]] = EBoardState::BLACK;
			}
			TTuple<FPos, int64> TempValue = MiniMax(SetUpPos[i], CurrDepth + 1, MaxDepth, NextTable, !CPUTurn, Alpha, Beta);
			if (TempValue.Value > Result.Value) {
				if (ClickedPos == FPos(-1, -1)) {
					Result.Key = SetUpPos[i];
				}
				else {
					Result.Key = ClickedPos;
				}
				Result.Value = TempValue.Value;
			}
			Alpha = FMath::Max(Result.Value, Alpha);
			if (Alpha >= Beta)
				break;
		}
		return Result;
	}
	else {
		TTuple<FPos, int64> Result = MakeTuple(ClickedPos, INT64_MAX);
		for (int i = 0; i < SetUpPos.Num(); i++) {
			TMap<FPos, EBoardState> NextTable = Table;
			NextTable[SetUpPos[i]] = EBoardState::WHITE;
			TArray<FPos> ReversePos = GetReversePos(SetUpPos[i], NextTable, CurrState);
			for (int j = 0; j < ReversePos.Num(); j++) {
				NextTable[ReversePos[j]] = EBoardState::WHITE;
			}
			TTuple<FPos, int64> TempValue = MiniMax(SetUpPos[i], CurrDepth + 1, MaxDepth, NextTable, !CPUTurn, Alpha, Beta);
			if (TempValue.Value < Result.Value) {
				if (ClickedPos == FPos(-1, -1)) {
					Result.Key = SetUpPos[i];
				}
				else {
					Result.Key = ClickedPos;
				}
				Result.Value = TempValue.Value;
			}
			Beta = FMath::Min(Result.Value, Beta);
			if (Alpha >= Beta)
				break;
		}
		return Result;
	}
}

void UPlayGame::AddBoardWeight(FPos NewPos)
{
	if (NewPos.PosY == 0 || NewPos.PosY == Instance->SizeOfBoard - 1) {
		if (NewPos.PosX == 0 || NewPos.PosX == Instance->SizeOfBoard - 1)
			BoardWeight.Add(NewPos, 10);
		else if (NewPos.PosX == 1 || NewPos.PosX == Instance->SizeOfBoard - 2)
			BoardWeight.Add(NewPos, 1);
		else if (NewPos.PosX == 2 || NewPos.PosX == Instance->SizeOfBoard - 3)
			BoardWeight.Add(NewPos, 3);
		else
			BoardWeight.Add(NewPos, 2);
	}
	else if (NewPos.PosY == 1 || NewPos.PosY == Instance->SizeOfBoard - 2) {
		if (NewPos.PosX == 0 || NewPos.PosX == Instance->SizeOfBoard - 1)
			BoardWeight.Add(NewPos, 1);
		else if (NewPos.PosX == 1 || NewPos.PosX == Instance->SizeOfBoard - 2)
			BoardWeight.Add(NewPos, -5);
		else
			BoardWeight.Add(NewPos, -1);
	}
	else if (NewPos.PosY == 2 || NewPos.PosY == Instance->SizeOfBoard - 3) {
		if (NewPos.PosX == 0 || NewPos.PosX == Instance->SizeOfBoard - 1)
			BoardWeight.Add(NewPos, 3);
		else if (NewPos.PosX == 1 || NewPos.PosX == Instance->SizeOfBoard - 2)
			BoardWeight.Add(NewPos, -1);
		else
			BoardWeight.Add(NewPos, 0);
	}
	else {
		if (NewPos.PosX == 0 || NewPos.PosX == Instance->SizeOfBoard - 1)
			BoardWeight.Add(NewPos, 2);
		else if (NewPos.PosX == 1 || NewPos.PosX == Instance->SizeOfBoard - 2)
			BoardWeight.Add(NewPos, -1);
		else
			BoardWeight.Add(NewPos, 0);
	}
}
