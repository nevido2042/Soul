// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RandomMoveService.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API URandomMoveService : public UBTService
{
	GENERATED_BODY()
	
public:
	URandomMoveService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
