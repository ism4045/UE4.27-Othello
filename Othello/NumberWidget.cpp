// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberWidget.h"
#include "Components/Image.h"

bool UNumberWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	AnimationFinished.Clear();
	AnimationFinished.BindUFunction(this, FName(FString(TEXT("ChangeTextureAndAppear"))));
	BindToAnimationFinished(Disappear, AnimationFinished);
	return false;
}

void UNumberWidget::ChangeNumber(int64 number)
{
	PlayAnimation(Disappear);
	Number = number;
}

void UNumberWidget::ChangeTextureAndAppear()
{
	NumberImage->SetVisibility(ESlateVisibility::Visible);
	ChangeTexture(Number);
	PlayAnimation(Appear);
}

void UNumberWidget::InitNumber(int64 number)
{
	ChangeTexture(number);
	FVector2D CurrImageSize = NumberImage->Brush.GetImageSize();
	NumberImage->SetBrushSize(FVector2D(CurrImageSize.X / 6, CurrImageSize.Y / 6));
}

void UNumberWidget::SetColorNumberToWhite()
{
	NumberImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));
}

void UNumberWidget::SetColorNumberToBlack()
{
	NumberImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f));
}


void UNumberWidget::ChangeTexture(int64 number)
{
	switch (number)
	{
	case 0:
		NumberImage->SetBrushResourceObject(NumberTextures.TextureZero);
		break;
	case 1:
		NumberImage->SetBrushResourceObject(NumberTextures.TextureOne);
		break;
	case 2:
		NumberImage->SetBrushResourceObject(NumberTextures.TextureTwo);
		break;
	case 3:
		NumberImage->SetBrushResourceObject(NumberTextures.TextureThree);
		break;
	case 4:
		NumberImage->SetBrushResourceObject(NumberTextures.TextureFour);
		break;
	case 5:
		NumberImage->SetBrushResourceObject(NumberTextures.TextureFive);
		break;
	case 6:
		NumberImage->SetBrushResourceObject(NumberTextures.TextureSix);
		break;
	case 7:
		NumberImage->SetBrushResourceObject(NumberTextures.TextureSeven);
		break;
	case 8:
		NumberImage->SetBrushResourceObject(NumberTextures.TextureEight);
		break;
	case 9:
		NumberImage->SetBrushResourceObject(NumberTextures.TextureNine);
		break;
	case -1:
		NumberImage->SetVisibility(ESlateVisibility::Hidden);
	default:
		break;
	}
}