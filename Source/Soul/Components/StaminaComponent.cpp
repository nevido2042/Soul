// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	//CurrentStamina = MaxStamina;
	// ...
	
}

void UStaminaComponent::DecreaseStamina(float InAmount)
{
	float Result = CurrentStamina - InAmount;
	
	if (Result < 0.f)
	{
		Result = 0.f;
	}

	CurrentStamina = Result;
}

