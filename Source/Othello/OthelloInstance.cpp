// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloInstance.h"
#include "BoardPart.h"
#include "Components/Button.h"
#include "Math/UnrealMathUtility.h"
void UOthelloInstance::Init() {
	Super::Init();

	SizeOfBoard = 4;
	LimitTime = 0;
	//Player's turn is always a late turn

	P1.Score = 2;
	P2.Score = 2;
	P1.CanSetUp = true;
	P2.CanSetUp = true;
	P1.Turn = true;
	P2.Turn = false;
}


void UOthelloInstance::SetPlayerScore(int64 PlayerOneScore, int64 PlayerTwoScore)
{
	P1.Score = PlayerOneScore; 
	P2.Score = PlayerTwoScore;
}
