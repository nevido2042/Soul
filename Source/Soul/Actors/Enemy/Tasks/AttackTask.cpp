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

        // 대상 액터의 위치를 가져옴
        FVector TargetLocation = TargetActor->GetActorLocation();
        FVector AICharacterLocation = AIController->GetPawn()->GetActorLocation();

        // 대상 액터를 바라보는 방향의 회전을 계산
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(AICharacterLocation, TargetLocation);

        // 현재 AI 캐릭터의 회전을 가져옴
        FRotator CurrentRotation = AIController->GetPawn()->GetActorRotation();

        // 현재 회전에서 Yaw 값만 LookAtRotation의 Yaw 값으로 업데이트
        FRotator NewRotation = FRotator(CurrentRotation.Pitch, LookAtRotation.Yaw, CurrentRotation.Roll);

        // AI 캐릭터의 회전을 새롭게 설정된 Yaw 값을 포함하여 업데이트
        AIController->GetPawn()->SetActorRotation(NewRotation);

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
