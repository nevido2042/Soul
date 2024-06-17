// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "CharacterController.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API ACharacterController : public APlayerController
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

protected:
    UPROPERTY(EditAnywhere)
    float TurnScale = 0.2f; // 회전 속도 스케일

    UPROPERTY(EditAnywhere)
    float LookUpScale = 0.2f; // 회전 속도 스케일
};
