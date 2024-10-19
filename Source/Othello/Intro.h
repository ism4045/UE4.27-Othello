// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Intro.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API UIntro : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	virtual bool Initialize() override;

	UFUNCTION()
	void GoToPlay();

	UFUNCTION()
	bool IsWriteSomething();

	UFUNCTION()
	ESlateVisibility VisibleText_SizeOfBoard();
	UFUNCTION()
	ESlateVisibility VisibleText_LimitTime();

	UFUNCTION()
	void QuitGame();
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> PlayWidget;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* SizeOfBoardTB;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* LimitTimeTB;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SizeOfBoard_ErrorText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LimitTime_ErrorText;
};
