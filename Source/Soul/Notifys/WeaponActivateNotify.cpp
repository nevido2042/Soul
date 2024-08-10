// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/WeaponActivateNotify.h"
#include "Actors/PlayerCharacter/PlayerCharacter.h"
#include "Actors/Weapons/Sword.h"

void UWeaponActivateNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter == nullptr) return;
	
	Sword = PlayerCharacter->GetSword();
	if (Sword == nullptr) return;

	Sword->BeginHitDetect();
	
}

void UWeaponActivateNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (PlayerCharacter == nullptr) return;
	if (Sword == nullptr) return;

	Sword->HitDetect();
	
	
}

void UWeaponActivateNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (PlayerCharacter == nullptr) return;
	if (Sword == nullptr) return;

	Sword->EndHitDetect();


}
