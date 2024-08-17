// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Enemy.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/EnemyHPBar.h"
#include "AnimInstances/EnemyAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Data/TraceData.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Perception/AISense_Damage.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(200.0f, 50.0f));
	HealthBarWidgetComponent->SetVisibility(false);

	LockOnTarget = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	LockOnTarget->SetupAttachment(RootComponent);

	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	LockOnWidgetComponent->SetupAttachment(LockOnTarget);

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->SetAutoActivate(false);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	HideLockOnWidget(true);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDie) return DamageAmount;

	if (HealthComponent)
	{
		HealthComponent->GetDamage(DamageAmount);

		if (StunnedMontage)
		{
			GetCharacterMovement()->StopMovementImmediately();
			StopAnimMontage();
			PlayAnimMontage(StunnedMontage);
		}

		ParticleSystem->ActivateSystem();
		AudioComponent->Play();

		// 데미지 이벤트 보고
		if (EventInstigator && DamageCauser)
		{
			UAISense_Damage::ReportDamageEvent(GetWorld(), this, EventInstigator, DamageAmount, DamageCauser->GetActorLocation(), GetActorLocation());
		}
	}
	
	if (UEnemyHealthBar* EnemyHPBar = Cast<UEnemyHealthBar>(HealthBarWidgetComponent->GetWidget()))
	{
		EnemyHPBar->UpdateHPBar(HealthComponent->CurrentHealth / HealthComponent->DefaultHealth);
		
		HealthBarWidgetComponent->SetVisibility(true);
		FTimerHandle HealthHideTimer;
		GetWorldTimerManager().SetTimer(HealthHideTimer, this, &AEnemy::HideHealthBar, 5.F, false);
	}

	if (HealthComponent->CurrentHealth <= 0.f)
	{
		//die
		bIsDie = true;

		StopAnimMontage();

		Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance())->SetDie(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();

		// Stop the behavior tree
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController && AIController->BrainComponent)
		{
			AIController->BrainComponent->StopLogic("Death");
		}

		LockOnTarget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageAmount;

}

void AEnemy::HideHealthBar()
{
	HealthBarWidgetComponent->SetVisibility(false);
}

void AEnemy::HideLockOnWidget(bool InBool)
{
	LockOnWidgetComponent->SetHiddenInGame(InBool);
}

void AEnemy::Attack()
{
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) return;

	if (AttackMontage == nullptr) return;

	GetController()->StopMovement();

	PlayAnimMontage(AttackMontage);
}

void AEnemy::BeginHitDetect()
{
	LastHitStart = GetMesh()->GetSocketLocation(TEXT("HitStart"));
	LastHitEnd = GetMesh()->GetSocketLocation(TEXT("HitEnd"));

	IgnoreActors.Add(this);
}

void AEnemy::HitDetect()
{
	FVector HitStart = GetMesh()->GetSocketLocation(TEXT("HitStart"));
	FVector HitEnd = GetMesh()->GetSocketLocation(TEXT("HitEnd"));

	struct FLastCache
	{
		FLastCache(AEnemy* InEnemy,
			const FVector& InStart, const FVector& InEnd)
			: Enemy(InEnemy), Start(InStart), End(InEnd) {}

		~FLastCache()
		{
			Enemy->LastHitStart = Start;
			Enemy->LastHitEnd = End;
		}
		AEnemy* Enemy = nullptr;
		FVector Start, End;
	}; FLastCache LastCache(this, HitStart, HitEnd);

	FHitResult HitResult;

	float CapsuleRadius = 30.0f;
	//float CapsuleHalfHeight = 100.0f;


	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), HitStart, HitEnd, CapsuleRadius, Hitable, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green, FLinearColor::Red, 0.5f);
	if (!bHit)
	{
		const FVector CenterStart = (LastHitStart + LastHitEnd) / 2.0;
		const FVector CenterEnd = (HitStart + HitEnd) / 2.0;
		FVector Dir = (HitEnd - HitStart);
		Dir.Normalize();

		double Distance = UKismetMathLibrary::Vector_Distance(HitStart, HitEnd) / 2.0;
		bHit = UKismetSystemLibrary::BoxTraceSingle(GetWorld(), CenterStart, CenterEnd,
			FVector(Distance, CapsuleRadius, CapsuleRadius), Dir.Rotation(), Hitable, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Blue, FLinearColor::Red, 0.5f);
	}

	if (bHit)
	{
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 40.f, nullptr, nullptr, nullptr);
		IgnoreActors.Add(HitResult.GetActor());
	}
}

void AEnemy::EndHitDetect()
{
	IgnoreActors.Empty();
}


