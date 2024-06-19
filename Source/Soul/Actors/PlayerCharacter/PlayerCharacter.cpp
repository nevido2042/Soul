// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &APlayerCharacter::OnMontageNotifyBegin);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRun == false)
	{
		Jog();
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::Attack()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	if (Anim->Montage_IsPlaying(AttackMontage))
	{
		AttackIndex = 1;
	}
	else
	{
		if (Anim->IsAnyMontagePlaying()/*Anim->Montage_IsPlaying(RollMontage)*/)
		{
			return;
		}

		if (GetCharacterMovement()->IsFalling())
		{
			return;
		}

		PlayAnimMontage(AttackMontage);
	}
}

void APlayerCharacter::RollOrDodge()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	if (Anim->IsAnyMontagePlaying())
	{
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (GetLastMovementInputVector().Rotation() == FRotator::ZeroRotator)
	{
		PlayAnimMontage(BackDodgeMontage);
		return;
	}
	
	{
		FRotator Rotation = GetLastMovementInputVector().Rotation();
		SetActorRotation(Rotation);

		PlayAnimMontage(RollMontage);
	}
}

void APlayerCharacter::Run()
{
	bRun = true;
	float& CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float Scale = 3.f;
	CurrentSpeed = FMathf::Lerp(CurrentSpeed, RunSpeed, GetWorld()->DeltaTimeSeconds * Scale);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Run"));
}

void APlayerCharacter::Jog()
{
	bRun = false;
	float& CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float Scale = 3.f;
	CurrentSpeed = FMathf::Lerp(CurrentSpeed, JogSpeed, GetWorld()->DeltaTimeSeconds * Scale);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Walk"));

}

void APlayerCharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	--AttackIndex;

	if (AttackIndex < 0)
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.35f, AttackMontage);
		AttackIndex = 0;
	}
}

