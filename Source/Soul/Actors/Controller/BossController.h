// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/EnemyController.h"
#include "BossController.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API ABossController : public AEnemyController
{
	GENERATED_BODY()
	
protected:
	virtual void OnTargetPerceptionUpdated(AActor* Actor, struct FAIStimulus Stimulus) override;

	virtual void OnTargetPerceptionForgotten(AActor* Actor) override;
};
