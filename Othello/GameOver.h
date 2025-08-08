// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StateEnums.h"
#include "GameOver.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API UGameOver : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CreateBoard();

	UFUNCTION()
	void SetBoardColor(int64 PosX, int64 PosY, class UBoardPart* Board_Part);

	UFUNCTION()
	void SetUpPart();

	UFUNCTION()
	void EnabledGameOver();

	UFUNCTION()
	void GoToIntro();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> Intro_BP;
	UPROPERTY()
	int64 P1Score;
	UPROPERTY()
	int64 P2Score;
	UPROPERTY()
	FPos P1Pos;
	UPROPERTY()
	FPos P2Pos;
	UPROPERTY(BlueprintReadOnly)
	int64 MaxFrame;

	TMap<FPos, class UBoardPart*> BoardMap;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BoardPartWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPaperFlipbook* AIFlipbook;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UPaperFlipbook* PlayerFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPaperFlipbook* FireworkFlipbook;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* SetGameOver;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UImage* FireWork1;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UImage* FireWork2;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* BoardPanel;

	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnIntro;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WinText;

	UPROPERTY()
	FTimerHandle SetUpHandle;

	UPROPERTY()
	class UOthelloInstance* Instance;
	UPROPERTY()
	class AOthelloGameModeBase* OthelloGameMode;
};
