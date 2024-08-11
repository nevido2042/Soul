// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/EnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Data/TeamData.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyController::AEnemyController()
{
    EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

    EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);
    EnemyPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AEnemyController::OnTargetPerceptionForgotten);

    //EnemyPerceptionComponent->bForgetStaleActors = true;
    //DefaultEngine.ini를 편집
    /*[/ Script / AIModule.AISystem]
        bForgetStaleActors = True*/

    SetGenericTeamId(FGenericTeamId(BTeam));
}

void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
    {
        BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
    }
}

void AEnemyController::OnTargetPerceptionForgotten(AActor* Actor)
{
    if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
    {
        // 타겟을 잊어버렸을 때 블랙보드 값 초기화
        BlackboardComp->ClearValue(TEXT("TargetActor"));
    }
}

ETeamAttitude::Type AEnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
    if (const APawn* OtherPawn = Cast<APawn>(&Other)) 
    {
        if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
        {
            FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
            if (OtherTeamID == GetGenericTeamId()) {
                return ETeamAttitude::Friendly;
            }
            else {
                return ETeamAttitude::Hostile;
            }
        }
    }

    return ETeamAttitude::Neutral;
}

void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
    if (UseBlackboard(BlackboardData, BlackboardComponent))
    {
        RunBehaviorTree(BehaviorTree);
    }
}
