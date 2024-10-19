// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StateEnums.h"
#include "Board.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API UBoard : public UUserWidget
{
	GENERATED_BODY()

	friend class UPlayGame;
public:
	UFUNCTION()
	virtual bool Initialize() override;
	UFUNCTION()
	virtual void NativeConstruct() override;
	UFUNCTION()
	void CreateButton(int64 Length);
	UFUNCTION()
	class UOthelloInstance* GetOthelloInstance();

	UFUNCTION()
	void SetBoardColor(int64 PosX, int64 PosY, class UBoardPart* Board_Part);

	UFUNCTION()
	void SetOwningClass(class UPlayGame* OwningClass);

	UFUNCTION()
	void UpdateReverse(TArray<FPos> ReversePos);

	UFUNCTION()
	void UpdateSetUp(TArray<FPos> SetUpPos);

	UFUNCTION()
	void DisableAllParts();
protected:
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* BoardPanel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BoardPartWidget;

	UPROPERTY()
	class UOthelloInstance* Instance;

	UPROPERTY()
	class UPlayGame* Owning;

	TMap<FPos, class UBoardPart*> BoardMap;
};
