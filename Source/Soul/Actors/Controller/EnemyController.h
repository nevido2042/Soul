// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "GenericTeamAgentInterface.h" //team

#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();
	
protected:
	UPROPERTY(EditAnywhere)
	class UAIPerceptionComponent* EnemyPerceptionComponent;

	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, struct FAIStimulus Stimulus);

	UFUNCTION()
	virtual void OnTargetPerceptionForgotten(AActor* Actor);

protected:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere)
	UBlackboardData* BlackboardData;
};
