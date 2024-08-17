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
            CurrentRotation, // ���� ȸ��
            TargetRotation,  // ��ǥ ȸ��
            0.01f,       // ������ ���� �ð�(��Ÿ Ÿ��)
            10.f      // ���� �ӵ�
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
        // Ʈ���̽��� �������� ������ �����մϴ�.
        FVector Start = PlayerRef->GetCamera()->GetComponentLocation();
        FVector End = Target->GetActorLocation();

        // Ʈ���̽��� �浹 �Ķ���� ����
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(PlayerRef); // �ڱ� �ڽ��� �����ϵ��� ����

        // ���� Ʈ���̽��� ����
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECC_Visibility,  // ����Ϸ��� Ʈ���̽� ä�� (��: ECC_Visibility)
            CollisionParams
        );

        // Ʈ���̽� ��� Ȯ��
        if (bHit)
        {
            // �浹 ������ ����� ���� �׷��� �ð������� Ȯ�� (���� ����)
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
            // Ʈ���̽��� ������ ���
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

    // Ʈ���̽��� ������ ���� ���� ���͸� �����մϴ�.
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(PlayerRef);

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
    float CompareFloat = 0.f;
    AActor* TempTargetLockOn = nullptr;

    for (AActor* Actor : Actors)
    {
        // Ʈ���̽��� �������� ������ �����մϴ�.
        FVector Start = PlayerRef->GetCamera()->GetComponentLocation();
        FVector End = Actor->GetActorLocation();

        // Ʈ���̽��� �浹 �Ķ���� ����
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(PlayerRef); // �ڱ� �ڽ��� �����ϵ��� ����

        // ���� Ʈ���̽��� ����
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECC_Visibility,  // ����Ϸ��� Ʈ���̽� ä�� (��: ECC_Visibility)
            CollisionParams
        );

        // Ʈ���̽� ����� ó��
        if (bHit)
        {
            // �浹 ��ġ�� ������ ����� ���� �׸��ϴ�.
            DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 5.0f, 0, 1.0f);
        }
        else
        {
            // �浹�� �߻����� ���� ��� �Ķ��� ����� ���� �׸��ϴ�.
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

