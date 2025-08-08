// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOver.h"
#include "OthelloInstance.h"
#include "BoardPart.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "OthelloGameModeBase.h"

void UGameOver::NativeConstruct()
{
	Super::NativeConstruct();
	Instance = Instance = Cast<UOthelloInstance>(GetWorld()->GetGameInstance());
	OthelloGameMode = Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(this));
	CreateBoard();
	P1Score = 0;
	P2Score = 0;
	P1Pos = FPos(0, 0);
	P2Pos = FPos(Instance->SizeOfBoard - 1, Instance->SizeOfBoard - 1);
	ReturnIntro->OnClicked.AddUniqueDynamic(this, &UGameOver::GoToIntro);
	GetWorld()->GetTimerManager().SetTimer(SetUpHandle, this, &UGameOver::SetUpPart, 0.1f, true, 1.0f);
}

void UGameOver::CreateBoard()
{
	for (int i = 0; i < Instance->SizeOfBoard; i++) {
		for (int j = 0; j < Instance->SizeOfBoard; j++) {
			class UBoardPart* BoardPart = Cast<UBoardPart>(CreateWidget(this, BoardPartWidget));
			UUniformGridSlot* UniformGridSlot = BoardPanel->AddChildToUniformGrid(BoardPart, i, j);

			if (UniformGridSlot && BoardPart) {
				UniformGridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				UniformGridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				SetBoardColor(i, j, BoardPart);
				BoardPart->BoardBtn->SetVisibility(ESlateVisibility::HitTestInvisible);
				BoardMap.Add(FPos(i, j), BoardPart);
				BoardPart->SetPosition(FPos(i, j));
			}
		}
	}
}

void UGameOver::SetBoardColor(int64 PosX, int64 PosY, UBoardPart* Board_Part)
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

void UGameOver::SetUpPart()
{
	if (Instance->P1.Score != 0) {
		if (P1Score < Instance->P1.Score) {
			P1Score++;
			BoardMap[P1Pos]->SetUpForGameOver(EBoardState::WHITE);
			P1Pos.PosY++;
			if (P1Pos.PosY >= Instance->SizeOfBoard) {
				P1Pos.PosX++;
				P1Pos.PosY = 0;
			}
		}
	}
	if (Instance->P2.Score != 0) {
		if (P2Score < Instance->P2.Score) {
			P2Score++;
			BoardMap[P2Pos]->SetUpForGameOver(EBoardState::BLACK);
			P2Pos.PosY--;
			if (P2Pos.PosY <= -1) {
				P2Pos.PosX--;
				P2Pos.PosY = Instance->SizeOfBoard - 1;
			}
		}
	}
	if (P1Score == Instance->P1.Score && P2Score == Instance->P2.Score) {
		GetWorld()->GetTimerManager().ClearTimer(SetUpHandle);
		EnabledGameOver();
	}
}

void UGameOver::EnabledGameOver() {
	bool IsWin = Instance->P1.Score >= Instance->P2.Score;
	FString str = IsWin ? "Player Win!" : "CPU Win...";
	if (Instance->P1.Score > Instance->P2.Score) {
		str = "Player Win!";
		IsWin = true;
	}
	else if (Instance->P1.Score == Instance->P2.Score) {
		str = "Draw!";
		IsWin = true;
	}
	else if (Instance->P1.Score < Instance->P2.Score) {
		str = "CPU Win...";
		IsWin = false;
	}
	WinText->SetText(FText::FromString(str));
	if (IsWin) {
		FireWork1->SetVisibility(ESlateVisibility::Visible);
		FireWork2->SetVisibility(ESlateVisibility::Visible);
		MaxFrame = 79;
	}
	else {
		MaxFrame = 30;
	}
	PlayAnimation(SetGameOver);
}

void UGameOver::GoToIntro() {
	OthelloGameMode->ChangeMainWidget(Intro_BP);
}