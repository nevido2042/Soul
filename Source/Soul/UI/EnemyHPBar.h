// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBar.generated.h"

/**
 * 
 */

class UProgressBar;
class UHealthComponent;

UCLASS()
class SOUL_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;

public:
	void UpdateHPBar(float InValue);
};
