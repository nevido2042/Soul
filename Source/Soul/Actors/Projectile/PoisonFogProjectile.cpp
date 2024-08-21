// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/PoisonFogProjectile.h"
#include "Components/PoisonFogComponent.h"

APoisonFogProjectile::APoisonFogProjectile()
{
    PoisonFogComponent = CreateDefaultSubobject<UPoisonFogComponent>("PoisonFogComponent");
}

void APoisonFogProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (PoisonFogComponent && PoisonFogComponent->PoisonFog)
    {
        FVector SpawnLocation = Hit.ImpactPoint; // 충돌 지점
        FRotator SpawnRotation = FRotator::ZeroRotator; // 기본 회전 값 (필요에 따라 조정)

        // 독 안개를 충돌 지점에 스폰
        GetWorld()->SpawnActor<AActor>(PoisonFogComponent->PoisonFog, SpawnLocation, SpawnRotation);
    }

    Destroy();
}
