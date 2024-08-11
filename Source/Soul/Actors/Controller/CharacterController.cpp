// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Controller/CharacterController.h"
#include "Actors/PlayerCharacter/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Data/TeamData.h"

ACharacterController::ACharacterController()
{
    SetGenericTeamId(FGenericTeamId(ATeam));
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
        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ACharacterController::Attack);
        EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Completed, this, &ACharacterController::Roll);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ACharacterController::Run);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ACharacterController::RunEnd);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacterController::Jump);
        EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Started, this, &ACharacterController::PressStrongAttack);
        EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Completed, this, &ACharacterController::ReleaseStrongAttack);


    }
}

void ACharacterController::MoveForward(const FInputActionValue& Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        //ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), Value.Get<float>());

        // find out which way is forward
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // add movement 
        ControlledPawn->AddMovementInput(ForwardDirection, Value.Get<float>());
    }
}

void ACharacterController::MoveRight(const FInputActionValue& Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        //ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), Value.Get<float>());

        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector 
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        ControlledPawn->AddMovementInput(RightDirection, Value.Get<float>());
    }
}

void ACharacterController::Turn(const FInputActionValue& Value)
{

    AddYawInput(Value.Get<float>() * TurnScale);
}

void ACharacterController::LookUp(const FInputActionValue& Value)
{
    AddPitchInput(Value.Get<float>() * LookUpScale);
}

void ACharacterController::Attack()
{
    Cast<APlayerCharacter>(GetPawn())->Attack();
}

void ACharacterController::Roll()
{
    Cast<APlayerCharacter>(GetPawn())->RollOrDodge();
}

void ACharacterController::Run()
{
    Cast<APlayerCharacter>(GetPawn())->Run();
}

void ACharacterController::RunEnd()
{
    Cast<APlayerCharacter>(GetPawn())->Jog();
}

void ACharacterController::Jump()
{
    Cast<APlayerCharacter>(GetPawn())->Jump();
}

void ACharacterController::PressStrongAttack()
{
    Cast<APlayerCharacter>(GetPawn())->SetStrongAttack(true);
}

void ACharacterController::ReleaseStrongAttack()
{
    Cast<APlayerCharacter>(GetPawn())->SetStrongAttack(false);
}
