// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UAnimMontage;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera = nullptr;

public:
	USpringArmComponent* GetSpringArm() { return SpringArm; }

public:
	void Attack();
	void Roll();
	void Run();
	void Walk();
private:
	bool bRun = false;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 600.f;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 200.f;

	int AttackIndex = 0;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimMontage* RollMontage = nullptr;

	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
};
