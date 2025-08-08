// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPart.h"
#include "PlayGame.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "OthelloInstance.h"

void UBoardPart::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AnimationFinished.Clear();
	AnimationFinished.BindUFunction(this, FName(FString(TEXT("ChangeTexture"))));
	BindToAnimationFinished(Reverse_Up, AnimationFinished);
	BoardBtn->OnClicked.AddUniqueDynamic(this, &UBoardPart::SetUp);
	PartState = EBoardState::NONE;
	PieceImage->SetVisibility(ESlateVisibility::Hidden);
	
	CanSetUp = false;
}

void UBoardPart::ReverseUp()
{
	PlayAnimation(Reverse_Up);
	if (PartState == EBoardState::BLACK)
		PartState = EBoardState::WHITE;
	else
		PartState = EBoardState::BLACK;
}

void UBoardPart::ChangeTexture()
{
	UObject* CurrentTexture = PieceImage->Brush.GetResourceObject();

	if (BlackTexture == CurrentTexture) 
		PieceImage->SetBrushResourceObject(WhiteTexture);
	else
		PieceImage->SetBrushResourceObject(BlackTexture);
	PieceImage->SetBrushSize(FVector2D(150, 150));
	ReverseDown();
}

void UBoardPart::ReverseDown()
{
	PlayAnimation(Reverse_Down);
}

void UBoardPart::SetUp()
{
	class UOthelloInstance* Instance = Cast<UOthelloInstance>(GetWorld()->GetGameInstance());

	if (Instance) {
		if (FuncCall_WhenBtnClicked->GetTurn()) {
			PieceImage->SetBrushResourceObject(WhiteTexture);
			PieceImage->SetBrushSize(FVector2D(150, 150));
			PartState = EBoardState::WHITE;
		}
		else {
			PieceImage->SetBrushResourceObject(BlackTexture);
			PieceImage->SetBrushSize(FVector2D(150, 150));
			PartState = EBoardState::BLACK;
		}
	}

	PieceImage->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(SetUp_Piece);
	BoardBtn->SetVisibility(ESlateVisibility::HitTestInvisible);
	FuncCall_WhenBtnClicked->BtnOnClicked(Pos);
}

void UBoardPart::IntializePart(UObject* Texture)
{
	PieceImage->SetBrushResourceObject(Texture);
	PieceImage->SetBrushSize(FVector2D(150, 150));
	PieceImage->SetVisibility(ESlateVisibility::Visible);
	BoardBtn->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (Texture == WhiteTexture)
		PartState = EBoardState::WHITE;
	else
		PartState = EBoardState::BLACK;
}

void UBoardPart::SetCheckImage()
{
	PieceImage->SetBrushResourceObject(SetUpTexture);
	PieceImage->SetBrushSize(FVector2D(50, 50));
	PieceImage->SetVisibility(ESlateVisibility::Visible);
}

void UBoardPart::SetFuncCallClass(UPlayGame* OwnBoard)
{
	FuncCall_WhenBtnClicked = OwnBoard;
}

void UBoardPart::SetUpForGameOver(EBoardState NewPartState)
{
	if (NewPartState == EBoardState::WHITE) {
		PieceImage->SetBrushResourceObject(WhiteTexture);
		PieceImage->SetBrushSize(FVector2D(150, 150));
		PartState = EBoardState::BLACK;
	}
	else if (NewPartState == EBoardState::BLACK) {
		PieceImage->SetBrushResourceObject(BlackTexture);
		PieceImage->SetBrushSize(FVector2D(150, 150));
		PartState = EBoardState::BLACK;
	}
	PieceImage->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(SetUp_Piece);
}
