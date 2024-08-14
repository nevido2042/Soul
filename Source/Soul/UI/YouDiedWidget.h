// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "YouDiedWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UYouDiedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void PlayFadeInOutAnimation();

private:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeInOut;

protected:
    UFUNCTION()
    void OnFadeInOutAnimationFinished();

    FWidgetAnimationDynamicEvent EndDelegate;
};
