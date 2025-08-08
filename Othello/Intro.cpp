// Fill out your copyright notice in the Description page of Project Settings.


#include "Intro.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "OthelloGameModeBase.h"
#include "StateEnums.h"
#include "OthelloInstance.h"

bool UIntro::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (PlayButton) {
		PlayButton->OnClicked.AddDynamic(this, &UIntro::GoToPlay);
		PlayButton->bIsEnabledDelegate.BindUFunction(this, "IsWriteSomething");
	}
	else
		return false;
	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &UIntro::QuitGame);
	}

	if (SizeOfBoard_ErrorText && LimitTime_ErrorText) {
		SizeOfBoard_ErrorText->VisibilityDelegate.BindUFunction(this,"VisibleText_SizeOfBoard");
		LimitTime_ErrorText->VisibilityDelegate.BindUFunction(this, "VisibleText_LimitTime");
	}
	else
		return false;

	return true;
}

void UIntro::GoToPlay()
{
	class UOthelloInstance* Instance = Cast<UOthelloInstance>(GetWorld()->GetGameInstance());
	if (Instance) {
		Instance->SetSizeOfBoard(FCString::Atoi64(*SizeOfBoardTB->GetText().ToString()));
		Instance->SetLimitTime(FCString::Atoi64(*LimitTimeTB->GetText().ToString()));
	}

	class AOthelloGameModeBase* result = Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (result) {
		result->ChangeMainWidget(PlayWidget);
	}
}

bool UIntro::IsWriteSomething()
{
	if (SizeOfBoardTB->GetText().ToString().Len() > 0 && SizeOfBoardTB->GetText().IsNumeric()
		&& LimitTimeTB->GetText().ToString().Len() > 0 && LimitTimeTB->GetText().IsNumeric()
		&& FCString::Atoi64(*SizeOfBoardTB->GetText().ToString()) >= 4
		&& FCString::Atoi64(*SizeOfBoardTB->GetText().ToString()) % 2 == 0
		&& FCString::Atoi64(*LimitTimeTB->GetText().ToString()) > 0)
		return true;
	return false;
}

ESlateVisibility UIntro::VisibleText_SizeOfBoard()
{
	if (((SizeOfBoardTB->GetText().IsNumeric() && FCString::Atoi64(*SizeOfBoardTB->GetText().ToString()) >= 4) && FCString::Atoi64(*SizeOfBoardTB->GetText().ToString()) % 2 == 0)|| SizeOfBoardTB->GetText().IsEmpty())
		return ESlateVisibility::Hidden;
	return ESlateVisibility::Visible;
}

ESlateVisibility UIntro::VisibleText_LimitTime()
{
	if ((LimitTimeTB->GetText().IsNumeric() && FCString::Atoi64(*LimitTimeTB->GetText().ToString()) > 0) || LimitTimeTB->GetText().IsEmpty())
		return ESlateVisibility::Hidden;

	return ESlateVisibility::Visible;
}

void UIntro::QuitGame() {
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ConsoleCommand("quit");
}