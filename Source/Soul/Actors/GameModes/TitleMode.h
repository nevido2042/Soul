// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleMode.generated.h"

/**
 * 
 */

class UUserWidget;

UCLASS()
class SOUL_API ATitleMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> TitleAsset;
	UUserWidget* Title = nullptr;

protected:
	void EnableUIMode();
};
