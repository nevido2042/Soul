// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetLockOnComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Data/TraceData.h"
#include "Actors/PlayerCharacter/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/Enemy/Enemy.h"

// Sets default values for this component's properties
UTargetLockOnComponent::UTargetLockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

    PlayerRef = Cast<APlayerCharacter>(GetOwner());
    //ensure(PlayerRef);
	// ...
}


// Called when the game starts
void UTargetLockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


float UTargetLockOnComponent::CheckHowCloseTargetIsToCenter(AActor* Actor)
{
    FVector CameraForward = PlayerRef->GetCamera()->GetForwardVector();
    FVector Start = PlayerRef->GetActorLocation();
    FVector Target = Actor->GetActorLocation();
    FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Start, Target);
    FVector Vec = Rot.Vector();
    
    return UKismetMathLibrary::Dot_VectorVector(CameraForward, Vec);
}

void UTargetLockOnComponent::TargetLockOnEvent()
{
    if (Cast<AEnemy>(TargetLockOn)->GetIsDie())
    {
        StopTargetLockOn();
        return;
    }

    FVector PlayerLoc = PlayerRef->GetActorLocation();
    FVector TargetLoc = TargetLockOn->GetActorLocation();

    float Dist = FVector::Dist(PlayerLoc, TargetLoc);

    if (Dist > LockOnRadius)
    {
        StopTargetLockOn();
        return;
    }
    else
    {
        FRotator CurrentRotation = PlayerRef->GetController()->GetControlRotation();
        FRotator TargetRotation = GetLockOnCameraRotation();

        FRotator InterpRotation = UKismetMathLibrary::RInterpTo(
            CurrentRotation, // 현재 회전
            TargetRotation,  // 목표 회전
            0.01f,       // 프레임 간의 시간(델타 타임)
            10.f      // 보간 속도
        );

        FRotator NewRot = FRotator(InterpRotation.Pitch, InterpRotation.Yaw, CurrentRotation.Roll);

        PlayerRef->GetController()->SetControlRotation(NewRot);
    }
}

FRotator UTargetLockOnComponent::GetLockOnCameraRotation()
{
    FVector Start = PlayerRef->GetCamera()->GetComponentLocation();

    FVector PlayerLoc = PlayerRef->GetActorLocation();
    FVector TargetLoc = TargetLockOn->GetActorLocation();

    float Dist = FVector::Dist(PlayerLoc, TargetLoc);

    FVector Target(TargetLoc.X, TargetLoc.Y, TargetLoc.Z - Dist / LockOnPitch);

    return UKismetMathLibrary::FindLookAtRotation(Start, Target);
}

AActor* UTargetLockOnComponent::GetTargetOnLeft()
{
    float CompareFloat = 0.f;
    AActor* TempTargetLockOn = nullptr;

    TArray<AActor*> Targets = TraceForTargets();
    for (AActor* Target : Targets)
    {
        // 트레이스의 시작점과 끝점을 설정합니다.
        FVector Start = PlayerRef->GetCamera()->GetComponentLocation();
        FVector End = Target->GetActorLocation();

        // 트레이스의 충돌 파라미터 설정
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(PlayerRef); // 자기 자신을 무시하도록 설정

        // 라인 트레이스를 수행
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECC_Visibility,  // 사용하려는 트레이스 채널 (예: ECC_Visibility)
            CollisionParams
        );

        // 트레이스 결과 확인
        if (bHit)
        {
            // 충돌 지점에 디버그 구를 그려서 시각적으로 확인 (선택 사항)
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.0f, 12, FColor::Red, false, 5.0f);

            FVector CameraRightVector = PlayerRef->GetCamera()->GetRightVector();
            
            double Dot = UKismetMathLibrary::Dot_VectorVector(CameraRightVector, HitResult.Normal);
            double ASINd = UKismetMathLibrary::DegAsin(Dot);

            if (TargetLockOn != Target && ASINd > 0.f)
            {
                float CheckFloat = CheckHowCloseTargetIsToCenter(Target);

                if (CompareFloat < CheckFloat)
                {
                    CompareFloat = CheckFloat;
                    TempTargetLockOn = Target;
                }
            }
        }
        else
        {
            // 트레이스가 실패한 경우
            UE_LOG(LogTemp, Warning, TEXT("No hit detected."));
        }

    }
    return TempTargetLockOn;
}

AActor* UTargetLockOnComponent::GetTargetOnRight()
{
    return nullptr;
}

void UTargetLockOnComponent::StopTargetLockOn()
{
    Cast<AEnemy>(TargetLockOn)->HideLockOnWidget(true);

    TargetLockOn = nullptr;

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    PlayerController->SetIgnoreLookInput(false);

    bLockOn = false;

    UCharacterMovementComponent* CharacterMovementComponent = PlayerRef->GetCharacterMovement();
    CharacterMovementComponent->bOrientRotationToMovement = true;
    CharacterMovementComponent->bUseControllerDesiredRotation = false;

    GetWorld()->GetTimerManager().ClearTimer(TargetLockOnEventHandle);
}

void UTargetLockOnComponent::SwitchTargetOnLeft()
{
    if (IsValid(TargetLockOn))
    {
        AActor* Actor = GetTargetOnLeft();
        if (IsValid(Actor))
        {
            TargetLockOn = Actor;
        }
    }
}

void UTargetLockOnComponent::SwitchTargetOnRight()
{
    if (IsValid(TargetLockOn))
    {
        AActor* Actor = GetTargetOnRight();
        if (IsValid(Actor))
        {
            TargetLockOn = Actor;
        }
    }
}

TArray<AActor*> UTargetLockOnComponent::TraceForTargets()
{
    FVector StartLocation = PlayerRef->GetActorLocation();
    //FVector EndLocation = StartLocation + FVector(0.f, 0.f, 1000.f);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(LockOnPawn);

    // 트레이스가 영향을 받지 않을 액터를 설정합니다.
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(PlayerRef);

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
    float CompareFloat = 0.f;
    AActor* TempTargetLockOn = nullptr;

    for (AActor* Actor : Actors)
    {
        // 트레이스의 시작점과 끝점을 설정합니다.
        FVector Start = PlayerRef->GetCamera()->GetComponentLocation();
        FVector End = Actor->GetActorLocation();

        // 트레이스의 충돌 파라미터 설정
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(PlayerRef); // 자기 자신을 무시하도록 설정

        // 라인 트레이스를 수행
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECC_Visibility,  // 사용하려는 트레이스 채널 (예: ECC_Visibility)
            CollisionParams
        );

        // 트레이스 결과를 처리
        if (bHit)
        {
            // 충돌 위치에 빨간색 디버그 선을 그립니다.
            DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 5.0f, 0, 1.0f);
        }
        else
        {
            // 충돌이 발생하지 않은 경우 파란색 디버그 선을 그립니다.
            DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 5.0f, 0, 1.0f);

            float CheckFloat = CheckHowCloseTargetIsToCenter(Actor);

            if (CompareFloat < CheckFloat)
            {
                CompareFloat = CheckFloat;
                TempTargetLockOn = Actor;
            }
        }
    }

    return TempTargetLockOn;
}

void UTargetLockOnComponent::TriggerTargetLockOn()
{
    TArray<AActor*> Actors = TraceForTargets();
    AActor* Actor = CheckforClosestTarget(Actors);
    if (Actor)
    {
        TargetLockOn = Actor;
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        PlayerController->SetIgnoreLookInput(true);
        bLockOn = true;

        UCharacterMovementComponent* CharacterMovementComponent = PlayerRef->GetCharacterMovement();
        CharacterMovementComponent->bOrientRotationToMovement = false;
        CharacterMovementComponent->bUseControllerDesiredRotation = true;

        GetWorld()->GetTimerManager().SetTimer(TargetLockOnEventHandle, FTimerDelegate::CreateUObject(this, &UTargetLockOnComponent::TargetLockOnEvent), GetWorld()->DeltaTimeSeconds, true);

        Cast<AEnemy>(Actor)->HideLockOnWidget(false);
    }
}

