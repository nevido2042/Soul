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
#include "Components/TimelineComponent.h"
#include "Components/StaminaComponent.h"
#include "DamageTypes/PoisonDamageType.h"
#include "Engine/DamageEvents.h"

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

	StaminaRegenerationLooperTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("StaminaRegenerationLooperTimeline"));
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

	if (StaminaCurve)
	{
		// Ÿ�Ӷ��ο� Ŀ�� ����
		OnTimelineUpdate.BindUFunction(this, FName("StaminaRegenTick"));
		OnTimelineFinished.BindUFunction(this, FName("HandleTimelineFinished"));

		StaminaRegenerationLooperTimeline->AddInterpFloat(StaminaCurve, OnTimelineUpdate);
		StaminaRegenerationLooperTimeline->SetTimelineFinishedFunc(OnTimelineFinished);

		// ���� ���� (�ʿ� ��)
		StaminaRegenerationLooperTimeline->SetLooping(true);

		//// Ÿ�Ӷ��� ����
		//StaminaRegenerationLooperTimeline->PlayFromStart();
	}
}

void APlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	TempPreviousMovementMode = PrevMovementMode;
	TempPreviousCustomMode = PreviousCustomMode;

	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	// ���ο� Movement Mode ��������
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

		// ������ �ð� ���
		float MontageDuration = HardLandingMontage->GetPlayLength();

		// ������ �� MovementMode �缳��
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

		/*if (SoulPlayerState->GetStaminaComponent()->CurrentStamina < 0.1f)
		{
			return;
		}*/

		SetAttackDamage(20.f);

		PlayAnimMontage(StrongAttackMontage);
	}
	else
	{
		if (Anim->IsAnyMontagePlaying())
		{
			return;
		}

		/*if (SoulPlayerState->GetStaminaComponent()->CurrentStamina < 0.1f)
		{
			return;
		}*/

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

	if (TryActionUseStamina(10.f) == false)
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
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) return;

	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking) return;

	if (bRun)
	{
		if (TryActionUseStamina(0.1f) == false)
		{
			Jog();
			return;
		}
	}

	if (!bRun) // ���°� �̹� Run ������ ���, �ӵ� ������ ����
	{
		bRun = true;
		// Set speed directly when changing to running
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void APlayerCharacter::Jog()
{
	if (bRun) // ���°� �̹� Jog ������ ���, �ӵ� ������ ����
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

	if (TryActionUseStamina(10.f) == false)
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

void APlayerCharacter::SwitchTargetOnLeft()
{
	TargetLockOnComponent->SwitchTargetOnLeft();
}

void APlayerCharacter::SwitchTargetOnRight()
{
	TargetLockOnComponent->SwitchTargetOnRight();
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
	// �̵� ��� �缳��
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APlayerCharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//if (TryActionUseStamina(10.f) == false)
	//{
	//	GetMesh()->GetAnimInstance()->Montage_Stop(0.35f);
	//	AttackIndex = 0;
	//	return;
	//}

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

		if (DamageEvent.DamageTypeClass != UPoisonDamageType::StaticClass())
		{
			GetCharacterMovement()->DisableMovement();
			float MontageDuration = ImpactMontage->GetPlayLength();
			PlayAnimMontage(ImpactMontage);
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::ResetMovementMode, MontageDuration, false);
			ParticleSystem->ActivateSystem();
			AudioComponent->Play();
		}

		if (SoulPlayerState->GetHealthComponent()->CurrentHealth <= 0.f)
		{
			bIsDie = true;
			//die
			StopAnimMontage();
			AudioComponent->Play();

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

void APlayerCharacter::RefreshStaminaRegeneration(bool bStartStaminaRegeneration)
{
	if (bStartStaminaRegeneration)
	{
		StaminaRegenerationLooperTimeline->PlayFromStart();
	}
	else
	{
		StaminaRegenerationLooperTimeline->Stop();
	}
}

void APlayerCharacter::StaminaRegenTick(float Value)
{
	// Ÿ�Ӷ��� ������Ʈ �� ȣ��� ����
	UE_LOG(LogTemp, Warning, TEXT("Timeline Update: %f"), Value);

	float CurrentStamina = SoulPlayerState->GetStaminaComponent()->CurrentStamina;
	float MaxStamina = SoulPlayerState->GetStaminaComponent()->MaxStamina;

	if (CurrentStamina >= MaxStamina)
	{
		RefreshStaminaRegeneration(false);
		SoulPlayerState->GetStaminaComponent()->CurrentStamina = MaxStamina;
	}
	else
	{
		float StaminaRegen = SoulPlayerState->GetStaminaComponent()->StaminaRegen;

		float Result = CurrentStamina + StaminaRegen;
		SoulPlayerState->GetStaminaComponent()->CurrentStamina = Result;
	}

	SoulHUD->UpdateStatusWidget();
	//RefreshStanimaBar();
}

void APlayerCharacter::HandleTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Timeline Finished"));
}

bool APlayerCharacter::TryActionUseStamina(float InCost)
{
	if (SoulPlayerState->GetStaminaComponent()->CurrentStamina < 3.f) //0.f���� �ϸ� �Ȱɸ��� ����
	{
		return false;
	}

	StaminaRegenerationLooperTimeline->Stop();
	SoulPlayerState->GetStaminaComponent()->DecreaseStamina(InCost);
	SoulHUD->UpdateStatusWidget();

	// 2�� �ڿ� StartRegenStamina �Լ��� �����մϴ�.
	GetWorld()->GetTimerManager().SetTimer(
		StartRegenStaminaHandle, // FTimerHandle ����
		this, // ȣ���� ��ü
		&APlayerCharacter::StartRegenStamina, // ȣ���� �Լ�
		2.0f, // ���� �ð� (��)
		false // �ݺ� ���� (false�� �����Ͽ� �� ���� ȣ��)
	);

	return true;
}

void APlayerCharacter::StartRegenStamina()
{
	RefreshStaminaRegeneration(true);
}

