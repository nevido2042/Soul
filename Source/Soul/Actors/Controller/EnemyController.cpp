// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/EnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Data/TeamData.h"
#include "EnemyController.h"

AEnemyController::AEnemyController()
{
    EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

    EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);

    SetGenericTeamId(FGenericTeamId(BTeam));
}

void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    //ETeamAttitude a = GetTeamAttitudeTowards(Actor);
}

//ETeamAttitude::Type AEnemyController::GetTeamAttitudeTowards(const AActor& Other) const
//{
//    return Super::GetTeamAttitudeTowards(Other);
//}
