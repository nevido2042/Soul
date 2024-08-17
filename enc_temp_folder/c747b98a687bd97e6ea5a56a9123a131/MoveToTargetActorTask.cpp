// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Tasks/MoveToTargetActorTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UMoveToTargetActorTask::UMoveToTargetActorTask()
{
	NodeName = "Move To Target Actor";
}

EBTNodeResult::Type UMoveToTargetActorTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
    if (TargetActor == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AIController->MoveToActor(TargetActor);

    return EBTNodeResult::Succeeded;
}
