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
			// 스태미나 사용 시도가 실패한 경우
			if (PlayerCharacter->TryActionUseStamina(20.f) == false)
			{
				// 현재 실행 중인 몽타주를 멈춤
				UAnimInstance* AnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
				if (AnimInstance /*&& AnimInstance->Montage_IsPlaying(nullptr)*/)
				{
					AnimInstance->Montage_Stop(0.35f); // 0.2초 동안 멈추도록 설정
					return;
				}
			}
			
		}
	}
}
