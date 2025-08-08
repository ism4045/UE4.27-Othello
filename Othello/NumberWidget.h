// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateEnums.h"
#include "Blueprint/UserWidget.h"
#include "NumberWidget.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API UNumberWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual bool Initialize() override;

	UFUNCTION()
	void ChangeNumber(int64 number);
	UFUNCTION()
	void ChangeTextureAndAppear();
	UFUNCTION()
	void ChangeTexture(int64 number);
	UFUNCTION()
	void InitNumber(int64 number);
	UFUNCTION()
	void SetColorNumberToWhite();
	UFUNCTION()
	void SetColorNumberToBlack();
public:
	UPROPERTY(EditAnywhere)
	FNumberTexture NumberTextures;

	UPROPERTY(meta = (BindWidget))
	class UImage* NumberImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Disappear;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Appear;

	UPROPERTY()
	FWidgetAnimationDynamicEvent AnimationFinished;

	UPROPERTY()
	int64 Number;
};
