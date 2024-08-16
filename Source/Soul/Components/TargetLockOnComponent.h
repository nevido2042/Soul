// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetLockOnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_API UTargetLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetLockOnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	float LockOnRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> LockOnClass;

	AActor* TargetLockOn;

	class APlayerCharacter* PlayerRef;

	bool bLockOn = false;
public:
	bool GetIsLockOn() { return bLockOn; }
	void TriggerTargetLockOn();
	void StopTargetLockOn();

protected:
	TArray<AActor*> TraceForTargets();
	AActor* CheckforClosestTarget(TArray<AActor*> Actors);
	float CheckHowCloseTargetIsToCenter(AActor* Actor);

	UFUNCTION()
	void TargetLockOnEvent();
	FRotator GetLockOnCameraRotation();
	FTimerHandle TargetLockOnEventHandle;
		
};
