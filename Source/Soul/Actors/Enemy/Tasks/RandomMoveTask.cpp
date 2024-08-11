// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Tasks/RandomMoveTask.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

URandomMoveTask::URandomMoveTask()
{
	NodeName = "Random Move";
}

EBTNodeResult::Type URandomMoveTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (ControlledPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    FVector Origin = ControlledPawn->GetActorLocation();
    FNavLocation RandomPoint;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    NavSys->GetRandomReachablePointInRadius(Origin, 1000.f, RandomPoint);
    
    OwnerComp.GetAIOwner()->MoveTo(RandomPoint.Location);


    return EBTNodeResult::Succeeded;
}
