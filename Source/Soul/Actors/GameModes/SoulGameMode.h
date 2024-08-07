// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SoulGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API ASoulGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	void HideMouseAndSetGameMode();
};
