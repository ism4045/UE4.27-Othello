// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateEnums.h"
#include "Engine/GameInstance.h"
#include "OthelloInstance.generated.h"

UCLASS()
class OTHELLO_API UOthelloInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	UFUNCTION()
	int64 GetSizeOfBoard() { return SizeOfBoard; }
	UFUNCTION()
	void SetSizeOfBoard(const int64 InputNum) { SizeOfBoard = InputNum; }

	UFUNCTION()
	int64 GetLimitTime() { return LimitTime; }
	UFUNCTION()
	void SetLimitTime(const int64 InputNum) { LimitTime = InputNum; }

	UFUNCTION()
	void SetPlayerScore(int64 PlayerOneScore, int64 PlayerTwoScore);

	UFUNCTION()
	FPlayerStatus GetP1Status() { return P1; }

	UFUNCTION()
	FPlayerStatus GetP2Status() { return P2; }

public:
	UPROPERTY()
	int64 SizeOfBoard;
	UPROPERTY()
	int64 LimitTime;

	UPROPERTY()
	FPlayerStatus P1;
	UPROPERTY()
	FPlayerStatus P2;

};
