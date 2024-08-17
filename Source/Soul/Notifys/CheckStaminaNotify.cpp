// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/CheckStaminaNotify.h"
#include "Actors/PlayerCharacter/PlayerCharacter.h"

void UCheckStaminaNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Actor = MeshComp->GetOwner();
	if (Actor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor);

		if (PlayerCharacter)
		{
			// ���¹̳� ��� �õ��� ������ ���
			if (PlayerCharacter->TryActionUseStamina(20.f) == false)
			{
				// ���� ���� ���� ��Ÿ�ָ� ����
				UAnimInstance* AnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
				if (AnimInstance /*&& AnimInstance->Montage_IsPlaying(nullptr)*/)
				{
					AnimInstance->Montage_Stop(0.35f); // 0.2�� ���� ���ߵ��� ����
					return;
				}
			}
			
		}
	}
}
