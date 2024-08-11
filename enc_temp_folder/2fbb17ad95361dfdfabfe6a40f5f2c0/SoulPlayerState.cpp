// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PlayerState/SoulPlayerState.h"
#include "Components/HealthComponent.h"

ASoulPlayerState::ASoulPlayerState()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}
