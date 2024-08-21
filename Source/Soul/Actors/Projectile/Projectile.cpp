// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    SetRootComponent(CollisionComponent);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;

    // Initialize projectile speeds
    BaseSpeed = 1000.0f; // Default speed
    MaxSpeed = 3000.0f; // Maximum speed
    MinSpeed = 500.0f;  // Minimum speed

    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Set up collision and movement
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor)
    {
        UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);
        Destroy();
    }
}

void AProjectile::SetDamage(float NewDamage)
{
    Damage = NewDamage;
}

void AProjectile::LaunchProjectile(const FVector& LaunchDirection, float TargetDistance)
{
    Speed = FMath::Clamp(BaseSpeed + (TargetDistance), MinSpeed, MaxSpeed);
    ProjectileMovementComponent->InitialSpeed = Speed;
    ProjectileMovementComponent->MaxSpeed = Speed;

    // Set the direction and launch the projectile
    ProjectileMovementComponent->Velocity = LaunchDirection * Speed;
}

