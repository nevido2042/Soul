// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Tasks/StopMovementTask.h"
#include "AIController.h"

UStopMovementTask::UStopMovementTask()
{
	NodeName = "Stop Movement";
}

EBTNodeResult::Type UStopMovementTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController)
    {
        AIController->StopMovement();
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}
