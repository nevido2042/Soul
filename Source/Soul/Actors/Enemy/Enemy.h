// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitDetectable.h"
#include "Enemy.generated.h"

class UHealthComponent;
class UWidgetComponent;

UCLASS()
class SOUL_API AEnemy : public ACharacter, public IHitDetectable<AEnemy>
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* HealthBarWidgetComponent;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
	void HideHealthBar();

protected:
	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage;
public:
	void Attack();

public:
	virtual void BeginHitDetect() override;
	virtual void HitDetect() override;
	virtual void EndHitDetect() override;

//protected:
//	friend class FLastCache;
//	FVector LastHitStart;
//	FVector LastHitEnd;
//
//	TArray<AActor*> IgnoreActors;
};
