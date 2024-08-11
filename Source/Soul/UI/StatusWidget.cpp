// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatusWidget.h"
#include "Components/ProgressBar.h"

void UStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatusWidget::SetHealthBar(float InValue)
{
	HealthBar->SetPercent(InValue);
}

void UStatusWidget::SetStaminaBar(float InValue)
{
	StaminaBar->SetPercent(InValue);
}
