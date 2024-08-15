// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetLockOnComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Data/TraceData.h"
#include "Actors/PlayerCharacter/PlayerCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UTargetLockOnComponent::UTargetLockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

    PlayerRef = Cast<APlayerCharacter>(GetOwner());
    ensure(PlayerRef);
	// ...
}


// Called when the game starts
void UTargetLockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


TArray<AActor*> UTargetLockOnComponent::TraceForTargets()
{
    FVector StartLocation = PlayerRef->GetActorLocation();
    //FVector EndLocation = StartLocation + FVector(0.f, 0.f, 1000.f);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(LockOnPawn);

    // 트레이스가 영향을 받지 않을 액터를 설정합니다.
    TArray<AActor*> IgnoreActors;

    // 트레이스 결과를 저장할 배열을 생성합니다.
    TArray<FHitResult> HitResults;

    TArray<AActor*> TargetLockOnActors;

    // Multi-Sphere Trace를 수행합니다.
    bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),
        StartLocation,
        StartLocation,
        LockOnRadius,
        ObjectTypes,
        false, // 단일 트레이스가 아닌 경우 false로 설정
        IgnoreActors,
        EDrawDebugTrace::ForDuration, // 트레이스 디버그 표시 유형
        HitResults,
        true // 히트된 경우에만 표시
    );

    for (FHitResult HitResult : HitResults)
    {
        if (HitResult.GetActor()->GetClass()->IsChildOf(LockOnClass))
        {
            TargetLockOnActors.Add(HitResult.GetActor());
        }
       
    }

    return TargetLockOnActors;
}

AActor* UTargetLockOnComponent::CheckforClosestTarget(TArray<AActor*> Actors)
{
    //// 트레이스의 시작점과 끝점을 설정합니다.
    //FVector Start = PlayerRef->GetCamera()->GetComponentLocation();
    //FVector End = Start + (GetActorForwardVector() * 1000.0f); // 1000.0f 만큼 전방으로 트레이스

    //// 트레이스의 충돌 파라미터 설정
    //FHitResult HitResult;
    //FCollisionQueryParams CollisionParams;
    //CollisionParams.AddIgnoredActor(this); // 자기 자신을 무시하도록 설정

    //// 라인 트레이스를 수행
    //bool bHit = GetWorld()->LineTraceSingleByChannel(
    //    HitResult,
    //    Start,
    //    End,
    //    ECC_Visibility,  // 사용하려는 트레이스 채널 (예: ECC_Visibility)
    //    CollisionParams
    //);

    //// 트레이스 결과를 처리
    //if (bHit)
    //{
    //    // HitResult에 충돌한 정보가 들어 있습니다.
    //    UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.Actor->GetName());

    //    // 충돌 위치에 빨간색 디버그 선을 그립니다.
    //    DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 5.0f, 0, 1.0f);
    //}
    //else
    //{
    //    // 충돌이 발생하지 않은 경우 파란색 디버그 선을 그립니다.
    //    DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 5.0f, 0, 1.0f);
    //}

    //return nullptr;
}

void UTargetLockOnComponent::TriggerTargetLockOn()
{
    TArray<AActor*> Actors = TraceForTargets();
    CheckforClosestTarget(Actors);
}

