// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SoulPlayerState.generated.h"

/**
 * 
 */

class UHealthComponent;

UCLASS()
class SOUL_API ASoulPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASoulPlayerState();

protected:
	UPROPERTY(EditAnywhere)
	UHealthComponent* HealthComponent;
	
public:
	UHealthComponent* GetHealthComponent();
};
