// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Title.generated.h"

/**
 * 
 */

class UButton;

UCLASS()
class SOUL_API UTitle : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* GameStartBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ExitBtn;

protected:
	UFUNCTION()
	void OnGameStartBtnClicked();
	UFUNCTION()
	void OnExitClicked();

};
