// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PlayerState/SoulPlayerState.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"


ASoulPlayerState::ASoulPlayerState()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
}

UHealthComponent* ASoulPlayerState::GetHealthComponent()
{
	return HealthComponent;
}

UStaminaComponent* ASoulPlayerState::GetStaminaComponent()
{
	return StaminaComponent;
}
