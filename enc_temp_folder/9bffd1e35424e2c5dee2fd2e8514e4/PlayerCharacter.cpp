// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/StatusWidget.h"
#include "Actors/Weapons/Sword.h"
#include "Components/HealthComponent.h"
#include "Actors/PlayerState/SoulPlayerState.h"
#include "HUD/SoulHUD.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Combo Attack
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &APlayerCharacter::OnMontageNotifyBegin);

	//Status Widget
	SoulPlayerState = Cast<ASoulPlayerState>(GetPlayerState());

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		SoulHUD = Cast<ASoulHUD>(PlayerController->GetHUD());
	}

	//Weapon
	AActor* WeaponActor = Weapon->GetChildActor();
	if (WeaponActor)
	{
		WeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	}
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

ASword* APlayerCharacter::GetSword()
{
	return Cast<ASword>(Weapon->GetChildActor());
}

void APlayerCharacter::Attack()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (Anim->Montage_IsPlaying(StrongAttackMontage))
	{
		AttackIndex = 1;
	}
	else if (Anim->Montage_IsPlaying(AttackMontage))
	{
		AttackIndex = 1;
	}
	else if (bStrongAttack)
	{
		if (Anim->IsAnyMontagePlaying())
		{
			return;
		}

		PlayAnimMontage(StrongAttackMontage);
	}
	else
	{
		if (Anim->IsAnyMontagePlaying())
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

void APlayerCharacter::Jump()
{
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	Super::Jump();
}

void APlayerCharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	--AttackIndex;

	if (AttackIndex < 0)
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.35f);
		AttackIndex = 0;
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (SoulPlayerState)
	{
		SoulPlayerState->GetHealthComponent()->GetDamage(DamageAmount);
	}
	if (SoulHUD)
	{
		SoulHUD->UpdateStatusWidget();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

