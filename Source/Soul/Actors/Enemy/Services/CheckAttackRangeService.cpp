// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Services/CheckAttackRangeService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UCheckAttackRangeService::UCheckAttackRangeService()
{
	NodeName = TEXT("Check Attack Range");
}

void UCheckAttackRangeService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return;

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
    AAIController* AIController = OwnerComp.GetAIOwner();
    AActor* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

    if (TargetActor && ControlledPawn)
    {
        float AttackRange = /*BlackboardComp->GetValueAsFloat("AttackRange");*/200.f;
        bool bIsInRange = IsTargetInRange(TargetActor, ControlledPawn, AttackRange);
        BlackboardComp->SetValueAsBool("bIsTargetInRange", bIsInRange);
    }
}

bool UCheckAttackRangeService::IsTargetInRange(AActor* TargetActor, AActor* ControlledPawn, float AttackRange) const
{
    if (!TargetActor || !ControlledPawn) return false;

    float Distance = FVector::Dist(TargetActor->GetActorLocation(), ControlledPawn->GetActorLocation());
    return Distance <= AttackRange;
}
