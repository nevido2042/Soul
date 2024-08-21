// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RangedAttackTask.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API URangedAttackTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	URangedAttackTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
