// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/Sword.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Data/TraceData.h"

// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	//AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	//AudioComponent->SetupAttachment(RootComponent);
	/*AudioComponent->SetSound(HitSound);*/
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	OwnerCont = Cast<AController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ASword::BeginHitDetect()
{
	LastHitStart = StaticMeshComponent->GetSocketLocation(TEXT("HitStart"));
	LastHitEnd = StaticMeshComponent->GetSocketLocation(TEXT("HitEnd"));

	IgnoreActors.Add(this);
	IgnoreActors.Add(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
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

	float CapsuleRadius = 10.0f;
	//float CapsuleHalfHeight = 100.0f;


	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), HitStart, HitEnd, CapsuleRadius, Hitable, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green, FLinearColor::Red, 0.5f);
	if (!bHit)
	{
		const FVector CenterStart = (LastHitStart + LastHitEnd) / 2.0;
		const FVector CenterEnd = (HitStart + HitEnd) / 2.0;
		FVector Dir = (HitEnd - HitStart);
		Dir.Normalize();

		double Distance = UKismetMathLibrary::Vector_Distance(HitStart, HitEnd) / 2.0;
		bHit = UKismetSystemLibrary::BoxTraceSingle(GetWorld(), CenterStart, CenterEnd,
			FVector(Distance, CapsuleRadius, CapsuleRadius), Dir.Rotation(), Hitable, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Blue, FLinearColor::Red, 0.5f);
	}

	if (bHit)
	{
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10.f, OwnerCont, OwnerActor, nullptr);
		IgnoreActors.Add(HitResult.GetActor());
		/*AudioComponent->Play();*/
	}
}

void ASword::EndHitDetect()
{
	IgnoreActors.Empty();
}


