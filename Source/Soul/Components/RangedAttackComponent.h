// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RangedAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_API URangedAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    URangedAttackComponent();

protected:
    virtual void BeginPlay() override;

public:
    void PerformRangedAttack(const FVector& LaunchDirection, float TargetDistance);
    void PlayAttackMontage();
private:
    UPROPERTY(EditAnywhere, Category = "Attack")
    TSubclassOf <class AProjectile > ProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackDamage;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackRange;

    UPROPERTY(EditAnywhere, Category = "Attack")
    UAnimMontage* AttackMontage;
};
