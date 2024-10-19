// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MouseController.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API AMouseController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
