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
    ProjectileMovementComponent->bShouldBounce = false;

    // Initialize projectile speeds
    BaseSpeed = 1000.0f; // Default speed
    MaxSpeed = 3000.0f; // Maximum speed
    MinSpeed = 500.0f;  // Minimum speed

    //PoisonFogComponent = CreateDefaultSubobject<UPoisonFogComponent>("PoisonFogComponent");
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    if (!OwnerActor)
    {
        OwnerActor = GetOwner();
    }

    SetLifeSpan(5.f);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    //if (PoisonFogComponent && PoisonFogComponent->PoisonFog)
    //{
    //    FVector SpawnLocation = Hit.ImpactPoint; // �浹 ����
    //    FRotator SpawnRotation = FRotator::ZeroRotator; // �⺻ ȸ�� �� (�ʿ信 ���� ����)

    //    // �� �Ȱ��� �浹 ������ ����
    //    GetWorld()->SpawnActor<AActor>(PoisonFogComponent->PoisonFog, SpawnLocation, SpawnRotation);
    //}

    Destroy();
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != OwnerActor)
    {
        UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);
        Destroy();
    }
}

//void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
//{
//    if (OtherActor && OtherActor != OwnerActor)
//    {
//        UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);
//        Destroy();
//    }
//}

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

