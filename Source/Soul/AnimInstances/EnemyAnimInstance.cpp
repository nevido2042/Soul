// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/EnemyAnimInstance.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Pawn = TryGetPawnOwner();
	if (!Pawn) { return; }
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
