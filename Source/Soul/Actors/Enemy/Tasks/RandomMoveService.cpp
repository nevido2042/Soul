#include "RandomMoveService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
//#include "GameFramework/Actor.h"

URandomMoveService::URandomMoveService()
{
    NodeName = "Random Move Service";
    Interval = 5.0f; // 5초마다 실행
}

void URandomMoveService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (ControlledPawn == nullptr)
    {
        return;
    }

    FVector Origin = ControlledPawn->GetActorLocation();
    FNavLocation RandomPoint;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSys && NavSys->GetRandomReachablePointInRadius(Origin, 1000.f, RandomPoint))
    {
        OwnerComp.GetAIOwner()->MoveToLocation(RandomPoint.Location);
    }
}
