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

	// ���ط�
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamagePerSecond;

	// Ž�� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* DetectionSphere;

	// ������ �̺�Ʈ
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ������ ���� �̺�Ʈ
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	// �������� ���� ���
	TArray<AActor*> OverlappedActors;

private:
	void ApplyDamageToOverlappedActors(float DeltaTime);


};
