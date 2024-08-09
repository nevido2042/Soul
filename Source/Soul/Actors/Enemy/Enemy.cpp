// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Enemy.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/EnemyHPBar.h"
#include "AnimInstances/EnemyAnimInstance.h"

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
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//test
	UGameplayStatics::ApplyDamage(this, 30.f, nullptr, nullptr, nullptr);
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
	}

	if (HealthComponent->CurrentHealth <= 0.f)
	{
		//die
		Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance())->SetDie(true);
	}

	return 0.0f;
}


