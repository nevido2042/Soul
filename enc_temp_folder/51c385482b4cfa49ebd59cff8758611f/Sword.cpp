// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/Sword.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
	IgnoreActors.Add(this);
}

void ASword::HitDetectStart()
{
	LastHitStart = StaticMeshComponent->GetSocketLocation(TEXT("HitStart"));
	LastHitEnd = StaticMeshComponent->GetSocketLocation(TEXT("HitEnd"));
}

void ASword::HitDetect()
{

	FVector HitStart = StaticMeshComponent->GetSocketLocation(TEXT("HitStart"));
	FVector HitEnd = StaticMeshComponent->GetSocketLocation(TEXT("HitEnd"));

	struct FLastCache
	{
		FLastCache(ASword* InWeapon,
			const FVector& InStart, const FVector& InEnd)
			: Weapon(InWeapon), Start(InStart), End(InEnd) {}

		~FLastCache()
		{
			Weapon->LastHitStart = Start;
			Weapon->LastHitEnd = End;
		}
		ASword* Weapon = nullptr;
		FVector Start, End;
	}; FLastCache LastCache(this, HitStart, HitEnd);

	FHitResult HitResult;
	//ĸ�� �õ� ��
	float CapsuleRadius = 50.0f;
	float CapsuleHalfHeight = 100.0f;

	//if (Owner)
	//{
	//	IgnoreActors.Add(Owner->GetPawn()); //when die can't find pawn

	//	if (AMnBCharacter* Char = Cast<AMnBCharacter>(Owner->GetPawn()))
	//	{
	//		if (Char->GetEquippeddShield())
	//		{
	//			IgnoreActors.Add(Char->GetEquippeddShield());
	//		}
	//	}
	//}
	//IgnoreActors.Add(this);

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), HitStart, HitEnd, 5.f, TraceTypeQuery4, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green, FLinearColor::Red, 0.5f);
	if (!bHit)
	{
		const FVector CenterStart = (LastHitStart + LastHitEnd) / 2.0;
		const FVector CenterEnd = (HitStart + HitEnd) / 2.0;
		FVector Dir = (HitEnd - HitStart);
		Dir.Normalize();

		double Distance = UKismetMathLibrary::Vector_Distance(HitStart, HitEnd) / 2.0;
		bHit = UKismetSystemLibrary::BoxTraceSingle(GetWorld(), CenterStart, CenterEnd,
			FVector(Distance, 5.f, 5.f), Dir.Rotation(), TraceTypeQuery4, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green, FLinearColor::Red, 0.5f);
	}

}

void ASword::HitDetectImpl(FHitResult& InHitResult)
{
	FVector HitStart = StaticMeshComponent->GetSocketLocation(TEXT("HitStart"));
	FVector HitEnd = StaticMeshComponent->GetSocketLocation(TEXT("HitEnd"));

	DrawDebugLine(
		GetWorld(),
		HitStart,
		InHitResult.Location,
		FColor::Red,
		false, 10.0f, 0, 1.0f
	);


	DrawDebugSphere(
		GetWorld(),
		InHitResult.Location,
		2.0f,
		24,
		FColor::Red,
		false, 10.0f
	);

	/*if (AWeapon* HitWeapon = Cast<AWeapon>(InHitResult.GetActor()))
	{
		if (Owner)
		{
			Owner->GetCharacter()->StopAnimMontage();
		}
		ParticleComponent->SetTemplate(Particles[1]);
		ParticleComponent->SetWorldLocation(InHitResult.ImpactPoint);
		ParticleComponent->ActivateSystem();

		SetRandomSoundAndPlay();

		return;
	}*/


	/*if (AShield* HitShield = Cast<AShield>(InHitResult.GetActor()))
	{
		AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(Owner->GetCharacter());
		if (MnBCharacter)
		{
			if (Owner)
			{
				Owner->GetCharacter()->StopAnimMontage();
				MnBCharacter->Blocked();

				ParticleComponent->SetTemplate(Particles[1]);
				ParticleComponent->SetWorldLocation(InHitResult.ImpactPoint);
				ParticleComponent->ActivateSystem();

				SetRandomSoundAndPlay();

				return;
			}

		}
	}*/

	/*ParticleComponent->SetTemplate(Particles[0]);
	ParticleComponent->SetWorldLocation(InHitResult.ImpactPoint);
	ParticleComponent->ActivateSystem();

	UGameplayStatics::ApplyDamage(InHitResult.GetActor(), 1, Owner, this, nullptr);

	bApplyDamage = true;*/
}


