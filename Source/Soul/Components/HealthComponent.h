// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float DefaultHealth = 100.f;

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

public:
    void GetDamage(float DamageAmount);
};
