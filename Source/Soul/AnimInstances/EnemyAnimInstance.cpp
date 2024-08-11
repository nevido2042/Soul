// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/EnemyAnimInstance.h"
#include "GameFramework/Character.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Pawn = TryGetPawnOwner();
	if (!Pawn) { return; }

    Character = Cast<ACharacter>(Pawn);
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Pawn) { return; }

    if (Character)
    {
        MoveSpeed = Character->GetVelocity().Size();
    }
}
