// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/CharacterController.h"
#include "Actors/PlayerCharacter/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
ACharacterController::ACharacterController()
{
}

void ACharacterController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(PlayerMappingContext, 1);
    }
}

void ACharacterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ACharacterController::MoveForward);
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ACharacterController::MoveRight);
        EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ACharacterController::Turn);
        EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ACharacterController::LookUp);
    }
}

void ACharacterController::MoveForward(const FInputActionValue& Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), Value.Get<float>());
    }
}

void ACharacterController::MoveRight(const FInputActionValue& Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), Value.Get<float>());
    }
}

void ACharacterController::Turn(const FInputActionValue& Value)
{
    /*if (APlayerCharacter* PC = Cast<APlayerCharacter>(GetPawn()))
    {
        PC->GetSpringArm()->AddRelativeRotation(FRotator(0.0f, Value.Get<float>() * TurnScale, 0.0f)); 
    }*/

    AddYawInput(Value.Get<float>() * TurnScale);
}

void ACharacterController::LookUp(const FInputActionValue& Value)
{
    /*if (APlayerCharacter* PC = Cast<APlayerCharacter>(GetPawn()))
    {
        PC->GetSpringArm()->AddRelativeRotation(FRotator(Value.Get<float>() * LookUpScale, 0.0f, 0.0f)); 
    }*/

    AddPitchInput(Value.Get<float>() * LookUpScale);
}
