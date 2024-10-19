// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateEnums.h"
#include "Blueprint/UserWidget.h"
#include "PlayGame.generated.h"

UCLASS()
class OTHELLO_API UPlayGame : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual bool Initialize() override;
	UFUNCTION()
	virtual void NativeConstruct() override;
	UFUNCTION()
	void InitGameSetting();

	UFUNCTION()
	void BtnOnClicked(FPos ClickedPos);
	
	UFUNCTION()
	void TurnChange();

	UFUNCTION()
	void UpdateScoreBox();

	UFUNCTION()
	void UpdateCurrentTurn(FPos ClickedPos);

	UFUNCTION()
	void WhenPassFinished();

	UFUNCTION()
	void GameOver();

	UFUNCTION()
	void SetBoardColor(int64 PosX, int64 PosY, class UBoardPart* Board_Part);

	UFUNCTION()
	void ProgressNextTurn();

	UFUNCTION()
	void ManageTurnTime();
	UFUNCTION()
	void UpdateTimerBox();
	UFUNCTION()
	void InitTimerBox();
	UFUNCTION()
	void ResetTimer();

	UFUNCTION()
	TArray<int64> GetNumberOfDigits(int64 num);
	UFUNCTION()
	void FillArray(int64 count, TArray<class UNumberWidget*>& NumberWidgetArray, class UHorizontalBox* Parent, bool NumberColor);

	UFUNCTION()
	TArray<FPos> GetReversePos(FPos ClickedPos, TMap<FPos, EBoardState> BoardMap, EBoardState CurrState);

	UFUNCTION()
	TArray<FPos> GetSetUpPos(TMap<FPos, EBoardState> BoardMap, EBoardState CurrState);

	UFUNCTION()
	bool CanSetUp(FPos DefaultPos, EBoardState CurrState, TMap<FPos, EBoardState> BoardMap);

	UFUNCTION()
	bool IsPass();

	UFUNCTION()
	void UpdateScore();

	UFUNCTION()
	void UpdateReverse(FPos ClickedPos);
	UFUNCTION()
	void UpdateSetUp();
	UFUNCTION()
	bool IsGameOver();
	UFUNCTION()
	bool GetTurn() { return Turn; }

	UFUNCTION()
	void AI_Execute();
	UFUNCTION()
	FPos AI_CalculatePos();

	TTuple<FPos,int64> MiniMax(FPos ClickedPos, int64 CurrDepth, int64 MaxDepth, TMap<FPos,EBoardState> Table, bool CPUTurn,int64 Alpha, int64 Beta);

	UFUNCTION()
	void AddBoardWeight(FPos NewPos);

	UFUNCTION()
	void GoToGameOverScreen();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameOver_BP;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> NumberWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> Board_BP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerTurnText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TurnText;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* TimerBox;
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* P1ScoreBox;
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* P2ScoreBox;
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* PlayBox;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* PassAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* GameOverAnim;

	UPROPERTY()
	FWidgetAnimationDynamicEvent AnimationFinished;

	UPROPERTY()
	class UOthelloInstance* Instance;
	UPROPERTY()
	class AOthelloGameModeBase* OthelloGameMode;

	UPROPERTY()
	FTimerHandle TurnLimitTimeHandle;
	UPROPERTY()
	FTimerHandle GameOverDelayHandle;
	UPROPERTY()
	FTimerHandle CPUHandle;

	TArray<class UNumberWidget*> TimerNumbers;
	TArray<class UNumberWidget*> P1ScoreNumbers;
	TArray<class UNumberWidget*> P2ScoreNumbers;

	UPROPERTY()
	class UBoard* Board;
	TMap<FPos, EBoardState> BoardStates;
	TMap<FPos, int64> BoardWeight;

	UPROPERTY()
	bool Turn;

	UPROPERTY()
	int64 CurrentTurnTime;
};


