// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RangedAttackComponent.h"
#include "Actors/Projectile/Projectile.h"
#include "GameFramework/Character.h"

URangedAttackComponent::URangedAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    AttackDamage = 25.0f; // Example damage
    AttackRange = 1000.0f; // Example range
}

void URangedAttackComponent::BeginPlay()
{
    Super::BeginPlay();
}

void URangedAttackComponent::PerformRangedAttack(const FVector& LaunchDirection, float TargetDistance)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !ProjectileClass) return;

    FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(TEXT("trailCenter"));
    FRotator SpawnRotation = Character->GetActorRotation();

    AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
    if (Projectile)
    {
        // Set projectile damage
        Projectile->SetDamage(AttackDamage);
        Projectile->LaunchProjectile(LaunchDirection, TargetDistance);
        // Set projectile speed or other properties if needed
    }
}

void URangedAttackComponent::PlayAttackMontage()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !ProjectileClass) return;

    if (AttackMontage == nullptr) return;
    Character->PlayAnimMontage(AttackMontage);
}
