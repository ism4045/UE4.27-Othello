// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
void AMouseController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
}
