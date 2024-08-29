// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoisonFog.generated.h"

UCLASS()
class SOUL_API APoisonFog : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoisonFog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* PoisonFog;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* Explosion;

	// 피해량
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamagePerSecond;

	// 탐지 영역
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* DetectionSphere;

	// 오버랩 이벤트
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 오버랩 종료 이벤트
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	// 오버랩한 액터 목록
	TArray<AActor*> OverlappedActors;

private:
	void ApplyDamageToOverlappedActors(float DeltaTime);


};
