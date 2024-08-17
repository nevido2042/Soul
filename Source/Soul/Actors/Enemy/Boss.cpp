// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Boss.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/EnemyHPBar.h"
#include "Components/HealthComponent.h"
#include "UI/BossHealthBar.h"

ABoss::ABoss()
{

    if (HealthBarWidgetComponent)
    {
        HealthBarWidgetComponent->DestroyComponent();
        HealthBarWidgetComponent = nullptr;
    }

}

void ABoss::BeginPlay()
{
    Super::BeginPlay();

}

void ABoss::CreateBossHealthBar()
{
    if (bIsDie) return;

    if (BossHealthBarAsset == nullptr) return;

    UUserWidget* Widget = CreateWidget(GetWorld(), BossHealthBarAsset);

    if (Widget)
    {
        if (BossHealthBar)
        {
            BossHealthBar->SetVisibility(ESlateVisibility::Visible);
            return;
        }

        BossHealthBar = Cast<UBossHealthBar>(Widget);
        if (BossHealthBar)
        {
            UpdateHealthBar();

            BossHealthBar->AddToViewport();
        }
    }
}

void ABoss::DestroyBossHealthBar()
{
    if (BossHealthBar)
    {
        BossHealthBar->SetVisibility(ESlateVisibility::Hidden);
    }
}

float ABoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    UpdateHealthBar();

    return DamageAmount;
}

void ABoss::Die()
{
    Super::Die();
    DestroyBossHealthBar();
}

void ABoss::UpdateHealthBar()
{
    if (BossHealthBar)
    {
        UUserWidget* HealthBar = BossHealthBar->GetHealthBar();

        if (UEnemyHealthBar* EnemyHealthBar = Cast<UEnemyHealthBar>(HealthBar))
        {
            EnemyHealthBar->UpdateHPBar(HealthComponent->CurrentHealth / HealthComponent->DefaultHealth);

        }

    }
}
