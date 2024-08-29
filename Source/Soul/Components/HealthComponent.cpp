// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	//DefaultHealth = 100.0f;
	//CurrentHealth = DefaultHealth;
	// ...
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	/*DefaultHealth = 100.0f;*/
	CurrentHealth = DefaultHealth;
}

void UHealthComponent::GetDamage(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, DefaultHealth);
	if (CurrentHealth <= 0.f)
	{
		//die
		
	}
}

