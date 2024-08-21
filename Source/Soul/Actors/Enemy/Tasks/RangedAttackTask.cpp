// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Tasks/RangedAttackTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/RangedAttackComponent.h"

URangedAttackTask::URangedAttackTask()
{
	NodeName = TEXT("Ranged Attack");
}

EBTNodeResult::Type URangedAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    AActor* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

    if (ControlledPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack!"));

        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));

        // ��� ������ ��ġ�� ������
        FVector TargetLocation = TargetActor->GetActorLocation();
        FVector AICharacterLocation = AIController->GetPawn()->GetActorLocation();

        /*float Dist = FVector::Dist(TargetLocation, AICharacterLocation);*/

        // ��� ���͸� �ٶ󺸴� ������ ȸ���� ���
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(AICharacterLocation, TargetLocation);

        // ���� AI ĳ������ ȸ���� ������
        FRotator CurrentRotation = AIController->GetPawn()->GetActorRotation();

        // ���� ȸ������ Yaw ���� LookAtRotation�� Yaw ������ ������Ʈ
        FRotator NewRotation = FRotator(CurrentRotation.Pitch, LookAtRotation.Yaw, CurrentRotation.Roll);

        // AI ĳ������ ȸ���� ���Ӱ� ������ Yaw ���� �����Ͽ� ������Ʈ
        AIController->GetPawn()->SetActorRotation(NewRotation);

        URangedAttackComponent* RangedAttackComponent = ControlledPawn->GetComponentByClass<URangedAttackComponent>();
        if (RangedAttackComponent == nullptr)  return EBTNodeResult::Failed;

        RangedAttackComponent->PlayAttackMontage();
        /*RangedAttackComponent->PerformRangedAttack(ControlledPawn->GetActorForwardVector(), Dist);*/

        // ������ ���������� ����Ǿ����� ��ȯ�մϴ�.
        return EBTNodeResult::Succeeded;
    }

    // ������ ���������� ��ȯ�մϴ�.
    return EBTNodeResult::Failed;
}
