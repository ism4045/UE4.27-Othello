// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"
#include "BoardPart.h"
#include "OthelloInstance.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"

bool UBoard::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	Instance = GetOthelloInstance();
	if (!Instance)
		return false;
	return false;
}

void UBoard::NativeConstruct()
{
	Super::NativeConstruct();
	CreateButton(Instance->GetSizeOfBoard());
}

void UBoard::CreateButton(int64 Length)
{
	for (int64 i = 0; i < Length; i++) {
		for (int64 j = 0; j < Length; j++) {
			class UBoardPart* BoardPart = Cast<UBoardPart>(CreateWidget(this, BoardPartWidget));

			UUniformGridSlot* UniformGridSlot = BoardPanel->AddChildToUniformGrid(BoardPart, i, j);

			if (UniformGridSlot) {
				UniformGridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				UniformGridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
			if (BoardPart) {
				SetBoardColor(i, j, BoardPart);
				if (i == Length / 2 - 1 && j == Length / 2 - 1)
					BoardPart->IntializePart(BoardPart->WhiteTexture);
				else if (i == Length / 2 - 1 && j == Length / 2)
					BoardPart->IntializePart(BoardPart->BlackTexture);
				else if (i == Length / 2 && j == Length / 2 - 1)
					BoardPart->IntializePart(BoardPart->BlackTexture);
				else if (i == Length / 2 && j == Length / 2)
					BoardPart->IntializePart(BoardPart->WhiteTexture);
				BoardPart->SetPosition(FPos(i, j));
				BoardPart->SetFuncCallClass(Owning);
				BoardMap.Add(FPos(i, j), BoardPart);
			}
		}
	}
}

UOthelloInstance* UBoard::GetOthelloInstance()
{
	return Cast<UOthelloInstance>(GetWorld()->GetGameInstance());;
}

void UBoard::SetBoardColor(int64 PosX, int64 PosY, UBoardPart* Board_Part)
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

void UBoard::SetOwningClass(UPlayGame* OwningClass)
{
	Owning = OwningClass;
}

void UBoard::UpdateReverse(TArray<FPos> ReversePos)
{
	for (int i = 0; i < ReversePos.Num(); i++) {
		BoardMap[ReversePos[i]]->ReverseUp();
	}
}

void UBoard::UpdateSetUp(TArray<FPos> SetUpPos)
{
	DisableAllParts();
	for (int i = 0; i < SetUpPos.Num(); i++) {
		BoardMap[SetUpPos[i]]->BoardBtn->SetVisibility(ESlateVisibility::Visible);
		BoardMap[SetUpPos[i]]->SetCheckImage();
		BoardMap[SetUpPos[i]]->CanSetUp = true;
	}
}

void UBoard::DisableAllParts()
{
	for (auto Iter = BoardMap.CreateIterator(); Iter; ++Iter) {
		if (Iter->Value->PartState == EBoardState::NONE) {
			Iter->Value->BoardBtn->SetVisibility(ESlateVisibility::HitTestInvisible);
			Iter->Value->PieceImage->SetVisibility(ESlateVisibility::Hidden);
			Iter->Value->CanSetUp = false;
		}
	}
}
