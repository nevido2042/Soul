// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Tasks/AttackTask.h"
#include "AIController.h"
#include "Actors/Enemy/Enemy.h"

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
