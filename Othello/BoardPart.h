// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateEnums.h"
#include "Blueprint/UserWidget.h"
#include "BoardPart.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API UBoardPart : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void ChangeTexture();

	UFUNCTION()
	void ReverseDown();

	UFUNCTION()
	void ReverseUp();

	UFUNCTION()
	void SetUp();

	UFUNCTION()
	void IntializePart(UObject* Texture);

	UFUNCTION()
	void SetCheckImage();

	UFUNCTION()
	void SetFuncCallClass(class UPlayGame* OwnBoard);

	UFUNCTION()
	void SetPosition(FPos NewPos) { Pos = NewPos; }

	UFUNCTION()
	void SetUpForGameOver(EBoardState PartState);
public:
	UPROPERTY(EditAnywhere)
	class UObject* WhiteTexture;
	UPROPERTY(EditAnywhere)
	class UObject* BlackTexture;
	UPROPERTY(EditAnywhere)
	class UObject* SetUpTexture;

	UPROPERTY(meta = (BindWidget))
	class UButton* BoardBtn;
	UPROPERTY(meta = (BindWidget))
	class UImage* PieceImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Reverse_Up;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Reverse_Down;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* SetUp_Piece;

	UPROPERTY()
	FWidgetAnimationDynamicEvent AnimationFinished;

	UPROPERTY()
	EBoardState PartState;

	UPROPERTY()
	class UPlayGame* FuncCall_WhenBtnClicked;

	UPROPERTY()
	bool CanSetUp;

	FPos Pos;
};
