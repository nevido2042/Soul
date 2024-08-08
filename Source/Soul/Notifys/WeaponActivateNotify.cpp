// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/WeaponActivateNotify.h"

void UWeaponActivateNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	MeshComp->GetOwner();
}

void UWeaponActivateNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT(__FUNCTION__));
}

void UWeaponActivateNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
}
