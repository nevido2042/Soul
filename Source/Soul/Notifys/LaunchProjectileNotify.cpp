// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/LaunchProjectileNotify.h"
#include "Components/RangedAttackComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void ULaunchProjectileNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;

    // ĳ������ AIController�� ��������
    AAIController* AIController = Cast<AAIController>(Character->GetController());
    if (AIController == nullptr) return;

    // AIController���� ������ ������Ʈ ��������
    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (BlackboardComp == nullptr) return;

    // �����忡�� Ÿ�� ���� ��������
    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
    if (TargetActor == nullptr) return;

    // ĳ������ RangedAttackComponent ��������
    URangedAttackComponent* RangedAttackComponent = Character->GetComponentByClass<URangedAttackComponent>();
    if (RangedAttackComponent == nullptr) return;

    // ���� ��ġ�� �� ��ġ ����
    FVector Start = Character->GetMesh()->GetSocketLocation(TEXT("trailCenter"));
    FVector End = TargetActor->GetActorLocation();

    // ���� ���� ���
    FVector Direction = End - Start;

    // ���� ������ ����(�Ÿ�) ���
    float Dist = Direction.Size();

    // ���� ���͸� ����ȭ (���̸� 1�� ����� ����)
    FVector NormalizedDirection = Direction.GetSafeNormal();

    // ����� �� �׸���
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 3.f);

    // ���Ÿ� ���� ����
    RangedAttackComponent->PerformRangedAttack(NormalizedDirection, Dist);
}
