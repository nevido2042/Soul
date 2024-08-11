// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RandomMoveTask.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API URandomMoveTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	URandomMoveTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
