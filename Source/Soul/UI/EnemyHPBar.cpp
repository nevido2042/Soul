// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyHPBar.h"
#include "Components/ProgressBar.h"

void UEnemyHealthBar::NativeConstruct()
{
}

void UEnemyHealthBar::UpdateHPBar(float InValue)
{
	HealthBar->SetPercent(InValue);
}
