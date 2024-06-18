// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
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

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::Attack()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	if (Anim->Montage_IsPlaying(RollMontage))
	{
		return;
	}

	if (Anim->Montage_IsPlaying(AttackMontage))
	{
		AttackIndex = 1;
	}
	else
	{
		PlayAnimMontage(AttackMontage);
	}
}

void APlayerCharacter::Roll()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	if (Anim->Montage_IsPlaying(RollMontage)|| Anim->Montage_IsPlaying(AttackMontage))
	{
		return;
	}
	else 
	{
		PlayAnimMontage(RollMontage);
	}
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

