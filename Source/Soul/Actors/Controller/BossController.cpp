// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/BossController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Actors/Enemy/Boss.h"

void ABossController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	Super::OnTargetPerceptionUpdated(Actor, Stimulus);

	ABoss* Boss = Cast<ABoss>(GetPawn());
	if (Boss)
	{
		Boss->CreateBossHealthBar();
		Boss->PlayBossBGM();
	}
}

void ABossController::OnTargetPerceptionForgotten(AActor* Actor)
{
	Super::OnTargetPerceptionForgotten(Actor);

	ABoss* Boss = Cast<ABoss>(GetPawn());
	if (Boss)
	{
		Boss->DestroyBossHealthBar();
		Boss->StopBossBGM();
	}
}
