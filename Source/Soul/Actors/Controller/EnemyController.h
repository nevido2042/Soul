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
	void OnTargetPerceptionUpdated(AActor* Actor, struct FAIStimulus Stimulus);

protected:
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	void SetGenericTeamId(const FGenericTeamId& NewTeamId) { TeamId = NewTeamId; }
	//virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
private:
	FGenericTeamId TeamId;
};
