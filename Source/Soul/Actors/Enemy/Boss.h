// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Enemy/Enemy.h"
#include "Boss.generated.h"

/**
 * 
 */
class UUserWidget;
class UEnemyHealthBar;
class UBossHealthBar;
UCLASS()
class SOUL_API ABoss : public AEnemy
{
	GENERATED_BODY()
	

public:
	ABoss();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BossHealthBarAsset;
	UBossHealthBar* BossHealthBar;

public:
	void CreateBossHealthBar();
	void DestroyBossHealthBar();

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die() override;
private:
	void UpdateHealthBar();
};
