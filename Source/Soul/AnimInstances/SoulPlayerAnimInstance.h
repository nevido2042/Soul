// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SoulPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API USoulPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	APawn* Pawn;
	class ACharacter* Character;

	UPROPERTY(BlueprintReadOnly)
	bool bDie = false;

	UPROPERTY(BlueprintReadOnly)
	float MoveSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool bJumping;

	UPROPERTY(BlueprintReadOnly)
	bool bFalling;

	UPROPERTY(BlueprintReadOnly)
	float MoveDirection;

	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;

public:
	void SetDie(bool InBool) { bDie = InBool; }
};
