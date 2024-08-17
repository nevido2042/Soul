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
#include "AnimInstances/SoulPlayerAnimInstance.h"
#include "Components/TargetLockOnComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);

	TargetLockOnComponent = CreateDefaultSubobject<UTargetLockOnComponent>(TEXT("TargetLockOnComponent"));

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->SetAutoActivate(false);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->SetAutoActivate(false);
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

	// OnMovementModeChanged 이벤트 바인딩
}

void APlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	TempPreviousMovementMode = PrevMovementMode;
	TempPreviousCustomMode = PreviousCustomMode;

	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	// 새로운 Movement Mode 가져오기
	EMovementMode NewMovementMode = GetCharacterMovement()->MovementMode;

	switch (NewMovementMode)
	{
	case MOVE_Walking:
		UE_LOG(LogTemp, Warning, TEXT("Character is now walking"));
		break;
	case MOVE_Falling:
		UE_LOG(LogTemp, Warning, TEXT("Character is now falling"));
		if (GetVelocity().Z < -1000.f)
		{
			bHardLanding = true;
		}
		else
		{
			// Start a timer to retry the movement mode change after 0.2 seconds
			GetWorld()->GetTimerManager().SetTimer(MovementModeTimerHandle, this, &APlayerCharacter::RetryMovementModeChange, 0.2f, false);
		}
		break;
	case MOVE_Swimming:
		UE_LOG(LogTemp, Warning, TEXT("Character is now swimming"));
		break;
	case MOVE_Flying:
		UE_LOG(LogTemp, Warning, TEXT("Character is now flying"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Character movement mode changed"));
		break;
	}

	if (bHardLanding && NewMovementMode == EMovementMode::MOVE_Walking)
	{
		GetCharacterMovement()->DisableMovement();
		PlayAnimMontage(HardLandingMontage);
		bHardLanding = false;

		// 딜레이 시간 계산
		float MontageDuration = HardLandingMontage->GetPlayLength();

		// 딜레이 후 MovementMode 재설정
		GetWorld()->GetTimerManager().SetTimer(HardLandingTimerHandle, this, &APlayerCharacter::ResetMovementMode, MontageDuration, false);
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

		SetAttackDamage(20.f);

		PlayAnimMontage(StrongAttackMontage);
	}
	else
	{
		if (Anim->IsAnyMontagePlaying())
		{
			return;
		}

		SetAttackDamage(10.f);

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
	if (!bRun) // 상태가 이미 Run 상태인 경우, 속도 조절을 방지
	{
		bRun = true;
		// Set speed directly when changing to running
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void APlayerCharacter::Jog()
{
	if (bRun) // 상태가 이미 Jog 상태인 경우, 속도 조절을 방지
	{
		bRun = false;
		// Set speed directly when changing to jogging
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
}

void APlayerCharacter::Jump()
{
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	Super::Jump();
}

void APlayerCharacter::TargetLockOn()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	if (TargetLockOnComponent->GetIsLockOn())
	{
		UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
		TargetLockOnComponent->StopTargetLockOn();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
		TargetLockOnComponent->TriggerTargetLockOn();
	}
}

void APlayerCharacter::SmoothTransitionSpeed(float TargetSpeed, float DeltaTime)
{
	float& CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float Scale = 3.f; // Adjust speed transition rate here
	CurrentSpeed = FMath::Lerp(CurrentSpeed, TargetSpeed, DeltaTime * Scale);
}

void APlayerCharacter::RetryMovementModeChange()
{
	OnMovementModeChanged(TempPreviousMovementMode, TempPreviousCustomMode);

}

void APlayerCharacter::ResetMovementMode()
{
	// 이동 모드 재설정
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
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
		if (bIsDie)
		{
			return DamageAmount;
		}
		SoulPlayerState->GetHealthComponent()->GetDamage(DamageAmount);

		ParticleSystem->ActivateSystem();
		AudioComponent->Play();

		GetCharacterMovement()->DisableMovement();
		float MontageDuration = ImpactMontage->GetPlayLength();
		PlayAnimMontage(ImpactMontage);
		GetWorld()->GetTimerManager().SetTimer(HardLandingTimerHandle, this, &APlayerCharacter::ResetMovementMode, MontageDuration, false);

		if (SoulPlayerState->GetHealthComponent()->CurrentHealth <= 0.f)
		{
			bIsDie = true;
			//die
			StopAnimMontage();

			Cast<USoulPlayerAnimInstance>(GetMesh()->GetAnimInstance())->SetDie(true);

			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			if (PlayerController)
			{
				PlayerController->DisableInput(PlayerController);

				SoulHUD->ShowYouDied();
			}

			GetCharacterMovement()->DisableMovement();

		}
	}
	if (SoulHUD)
	{
		SoulHUD->UpdateStatusWidget();
	}

	return DamageAmount;
}

