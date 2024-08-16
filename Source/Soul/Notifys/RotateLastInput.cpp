// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/RotateLastInput.h"
#include "Actors/PlayerCharacter/PlayerCharacter.h"
#include "Components/TargetLockOnComponent.h"

void URotateLastInput::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerActor);
		if (PlayerCharacter)
		{
			//if Lock on No Rotate
			if (PlayerCharacter->GetTargetLockOnComponent()->GetIsLockOn()) return;

			FVector NewVector = PlayerCharacter->GetLastMovementInputVector();
			PlayerCharacter->SetActorRotation(NewVector.ToOrientationQuat());
		}
	}
}
