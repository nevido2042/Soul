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
			if (PlayerCharacter->GetTargetLockOnComponent()->GetIsLockOn())
			{
				APawn* Pawn = Cast<APawn>(OwnerActor);
				FRotator CurRot = OwnerActor->GetActorRotation();
				FRotator NewRot;
				if (Pawn)
				{
					NewRot = Pawn->GetController()->GetControlRotation();
					NewRot = FRotator(CurRot.Pitch, NewRot.Yaw, CurRot.Roll);
				}
				
				PlayerCharacter->SetActorRotation(NewRot);
			}
			else
			{
				FVector NewVector = PlayerCharacter->GetLastMovementInputVector();

				// �Է��� ������ OwnerActor�� Forward Vector�� ���
				if (NewVector.IsNearlyZero())
				{
					NewVector = OwnerActor->GetActorForwardVector();
				}

				PlayerCharacter->SetActorRotation(NewVector.ToOrientationQuat());
			}

		}
	}
}
