// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Tasks/AttackTask.h"
#include "AIController.h"
#include "Actors/Enemy/Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UAttackTask::UAttackTask()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    AActor* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

    if (ControlledPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack!"));

        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));

        // Calculate the rotation to look at the target actor
        FVector TargetLocation = TargetActor->GetActorLocation();
        FVector AICharacterLocation = AIController->GetPawn()->GetActorLocation();
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(AICharacterLocation, TargetLocation);

        // Set the AI character's rotation to look at the target actor
        AIController->GetPawn()->SetActorRotation(LookAtRotation);

        AEnemy* Enemy = Cast<AEnemy>(ControlledPawn);
        if (Enemy)
        {
            Enemy->Attack();
        }

        // ������ ���������� ����Ǿ����� ��ȯ�մϴ�.
        return EBTNodeResult::Succeeded;
    }

    // ������ ���������� ��ȯ�մϴ�.
    return EBTNodeResult::Failed;
}
