// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	APawn* Pawn;
	
	UPROPERTY(BlueprintReadOnly)
	bool bDie = false;

	UPROPERTY(BlueprintReadOnly)
	float MoveSpeed;

public:
	void SetDie(bool InBool) { bDie = InBool; }
};
