// Copyright Epic Games, Inc. All Rights Reserved.


#include "OthelloGameModeBase.h"
#include "OthelloInstance.h"
#include "BoardPart.h"
#include "Components/Button.h"
void AOthelloGameModeBase::ChangeMainWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AOthelloGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    ChangeMainWidget(StartingWidgetClass);
    Instance = Cast<UOthelloInstance>(GetWorld()->GetGameInstance());
}
