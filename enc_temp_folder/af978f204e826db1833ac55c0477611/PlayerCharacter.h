// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SOUL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera = nullptr;

public:
	USpringArmComponent* GetSpringArm() { return SpringArm; }

public:
	void Attack();

private:
	int AttackIndex = 0;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage = nullptr;

	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
};
