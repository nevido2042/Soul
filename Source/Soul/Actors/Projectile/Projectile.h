// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class SOUL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
    AProjectile();

    void SetDamage(float NewDamage);
    void LaunchProjectile(const FVector& LaunchDirection, float TargetDistance);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    AActor* OwnerActor;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Damage")
    float Damage;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float Speed;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float BaseSpeed;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float MaxSpeed;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float MinSpeed;

protected  :
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UProjectileMovementComponent* ProjectileMovementComponent;

    //UPROPERTY(VisibleAnywhere, Category = "Components")
    //class UPoisonFogComponent* PoisonFogComponent;

    UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
