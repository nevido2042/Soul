// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/SoulPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void USoulPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Pawn = TryGetPawnOwner();

	if (!Pawn) { return; }

    Character = Cast<ACharacter>(Pawn);
}

void USoulPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Pawn) { return; }

    if (Character)
    {
        MoveSpeed = Character->GetVelocity().Length();

        Velocity = Character->GetVelocity();

        bJumping = !Character->CanJump();

        bFalling = Character->GetCharacterMovement()->IsFalling();

        MoveDirection = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
    }
}
