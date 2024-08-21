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

    // 캐릭터의 AIController를 가져오기
    AAIController* AIController = Cast<AAIController>(Character->GetController());
    if (AIController == nullptr) return;

    // AIController에서 블랙보드 컴포넌트 가져오기
    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (BlackboardComp == nullptr) return;

    // 블랙보드에서 타겟 액터 가져오기
    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
    if (TargetActor == nullptr) return;

    // 캐릭터의 RangedAttackComponent 가져오기
    URangedAttackComponent* RangedAttackComponent = Character->GetComponentByClass<URangedAttackComponent>();
    if (RangedAttackComponent == nullptr) return;

    // 시작 위치와 끝 위치 설정
    FVector Start = Character->GetMesh()->GetSocketLocation(TEXT("trailCenter"));
    FVector End = TargetActor->GetActorLocation();

    // 방향 벡터 계산
    FVector Direction = End - Start;

    // 방향 벡터의 길이(거리) 계산
    float Dist = Direction.Size();

    // 방향 벡터를 정규화 (길이를 1로 만드는 과정)
    FVector NormalizedDirection = Direction.GetSafeNormal();

    // 디버그 선 그리기
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 3.f);

    // 원거리 공격 수행
    RangedAttackComponent->PerformRangedAttack(NormalizedDirection, Dist);
}
