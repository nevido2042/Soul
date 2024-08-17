// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Tasks/MoveToTargetActorTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

    // Calculate the rotation to look at the target actor
    FVector TargetLocation = TargetActor->GetActorLocation();
    FVector AICharacterLocation = AIController->GetPawn()->GetActorLocation();
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(AICharacterLocation, TargetLocation);

    // Set the AI character's rotation to look at the target actor
    AIController->GetPawn()->SetActorRotation(LookAtRotation);

    return EBTNodeResult::Succeeded;
}
