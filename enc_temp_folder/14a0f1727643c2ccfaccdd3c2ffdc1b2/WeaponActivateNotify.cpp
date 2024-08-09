// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/WeaponActivateNotify.h"
#include "Actors/PlayerCharacter/PlayerCharacter.h"
#include "Actors/Weapons/Sword.h"

void UWeaponActivateNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());

	Sword = PlayerCharacter->GetSword();
	Sword->HitDetectStart();
}

void UWeaponActivateNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Sword->HitDetect();
}

void UWeaponActivateNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
}
