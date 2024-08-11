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
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(DamageAmount);
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
	}

	return 0.0f;
}

void AEnemy::HideHealthBar()
{
	HealthBarWidgetComponent->SetVisibility(false);
}


