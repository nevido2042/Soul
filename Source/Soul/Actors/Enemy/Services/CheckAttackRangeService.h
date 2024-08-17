// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckAttackRangeService.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UCheckAttackRangeService : public UBTService
{
	GENERATED_BODY()
	
public:
	UCheckAttackRangeService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float AttackRange = 150.0f;

private:
	bool IsTargetInRange(AActor* TargetActor, AActor* ControlledPawn, float InAttackRange) const;

};
