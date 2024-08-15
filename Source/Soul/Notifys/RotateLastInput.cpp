// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/RotateLastInput.h"
#include "Actors/PlayerCharacter/PlayerCharacter.h"

void URotateLastInput::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerActor);

		if (PlayerCharacter)
		{
			FVector NewVector = PlayerCharacter->GetLastMovementInputVector();
			PlayerCharacter->SetActorRotation(NewVector.ToOrientationQuat());
		}
	}
}
