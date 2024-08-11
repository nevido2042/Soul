// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/EnemyAttackNotify.h"
#include "Actors/Enemy/Enemy.h"

void UEnemyAttackNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    Enemy = Cast<AEnemy>(MeshComp->GetOwner());
    if (Enemy == nullptr) return;

    Enemy->BeginHitDetect();
}

void UEnemyAttackNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    if (Enemy == nullptr) return;

    Enemy->HitDetect();
}

void UEnemyAttackNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (Enemy == nullptr) return;

    Enemy->EndHitDetect();
}
