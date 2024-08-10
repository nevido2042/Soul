// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "GenericTeamAgentInterface.h" //team

#include "CharacterController.generated.h"

/**
 * 
 */

UCLASS()
class SOUL_API ACharacterController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
    ACharacterController();

protected:
    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

private:
    void MoveForward(const FInputActionValue& Value);
    void MoveRight(const FInputActionValue& Value);
    void Turn(const FInputActionValue& Value);
    void LookUp(const FInputActionValue& Value);
    void Attack();
    void Roll();
    void Run();
    void RunEnd();
    void Jump();
    void PressStrongAttack();
    void ReleaseStrongAttack();


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* PlayerMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveForwardAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveRightAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* TurnAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* LookUpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* AttackAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* RollAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* RunAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* TargetLockOnAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* StrongAttackAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

protected:
    UPROPERTY(EditAnywhere)
    float TurnScale = 0.2f; // ȸ�� �ӵ� ������

    UPROPERTY(EditAnywhere)
    float LookUpScale = 0.2f; // ȸ�� �ӵ� ������

protected:
    virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
    void SetGenericTeamId(const FGenericTeamId& NewTeamId) { TeamId = NewTeamId; }
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const

private:
	FGenericTeamId TeamId;
};
