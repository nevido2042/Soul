// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class SOUL_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	/*UPROPERTY(EditAnywhere)
	USoundBase* HitSound = nullptr;*/

	UPROPERTY(EditAnywhere)
	class UAudioComponent* AudioComponent = nullptr;

public:
	void BeginHitDetect();
	void HitDetect();
	void EndHitDetect();

protected:
	/*void HitDetectImpl(FHitResult& InHitResult);*/
	friend class FLastCache;
	FVector LastHitStart;
	FVector LastHitEnd;

	TArray<AActor*> IgnoreActors;

};
