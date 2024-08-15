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

    // Ʈ���̽��� ������ ���� ���� ���͸� �����մϴ�.
    TArray<AActor*> IgnoreActors;

    // Ʈ���̽� ����� ������ �迭�� �����մϴ�.
    TArray<FHitResult> HitResults;

    TArray<AActor*> TargetLockOnActors;

    // Multi-Sphere Trace�� �����մϴ�.
    bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),
        StartLocation,
        StartLocation,
        LockOnRadius,
        ObjectTypes,
        false, // ���� Ʈ���̽��� �ƴ� ��� false�� ����
        IgnoreActors,
        EDrawDebugTrace::ForDuration, // Ʈ���̽� ����� ǥ�� ����
        HitResults,
        true // ��Ʈ�� ��쿡�� ǥ��
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
    //// Ʈ���̽��� �������� ������ �����մϴ�.
    //FVector Start = PlayerRef->GetCamera()->GetComponentLocation();
    //FVector End = Start + (GetActorForwardVector() * 1000.0f); // 1000.0f ��ŭ �������� Ʈ���̽�

    //// Ʈ���̽��� �浹 �Ķ���� ����
    //FHitResult HitResult;
    //FCollisionQueryParams CollisionParams;
    //CollisionParams.AddIgnoredActor(this); // �ڱ� �ڽ��� �����ϵ��� ����

    //// ���� Ʈ���̽��� ����
    //bool bHit = GetWorld()->LineTraceSingleByChannel(
    //    HitResult,
    //    Start,
    //    End,
    //    ECC_Visibility,  // ����Ϸ��� Ʈ���̽� ä�� (��: ECC_Visibility)
    //    CollisionParams
    //);

    //// Ʈ���̽� ����� ó��
    //if (bHit)
    //{
    //    // HitResult�� �浹�� ������ ��� �ֽ��ϴ�.
    //    UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.Actor->GetName());

    //    // �浹 ��ġ�� ������ ����� ���� �׸��ϴ�.
    //    DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 5.0f, 0, 1.0f);
    //}
    //else
    //{
    //    // �浹�� �߻����� ���� ��� �Ķ��� ����� ���� �׸��ϴ�.
    //    DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 5.0f, 0, 1.0f);
    //}

    //return nullptr;
}

void UTargetLockOnComponent::TriggerTargetLockOn()
{
    TArray<AActor*> Actors = TraceForTargets();
    CheckforClosestTarget(Actors);
}

