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

        // 공격이 성공적으로 실행되었음을 반환합니다.
        return EBTNodeResult::Succeeded;
    }

    // 공격이 실패했음을 반환합니다.
    return EBTNodeResult::Failed;
}
