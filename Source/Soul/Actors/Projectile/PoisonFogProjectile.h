// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectile/Projectile.h"
#include "PoisonFogProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API APoisonFogProjectile : public AProjectile
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UPoisonFogComponent* PoisonFogComponent;
public:

	APoisonFogProjectile();

protected:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
