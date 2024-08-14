// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/YouDiedWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"

void UYouDiedWidget::PlayFadeInOutAnimation()
{
    EndDelegate.BindDynamic(this, &UYouDiedWidget::OnFadeInOutAnimationFinished);

    if (FadeInOut)
    {
        BindToAnimationFinished(FadeInOut, EndDelegate);
        PlayAnimation(FadeInOut);
    }
}

void UYouDiedWidget::OnFadeInOutAnimationFinished()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("TitleMap"));
}

