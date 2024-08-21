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
        FVector SpawnLocation = Hit.ImpactPoint; // �浹 ����
        FRotator SpawnRotation = FRotator::ZeroRotator; // �⺻ ȸ�� �� (�ʿ信 ���� ����)

        // �� �Ȱ��� �浹 ������ ����
        GetWorld()->SpawnActor<AActor>(PoisonFogComponent->PoisonFog, SpawnLocation, SpawnRotation);
    }

    Destroy();
}
