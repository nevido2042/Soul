// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/PoisonFog/PoisonFog.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DamageTypes/PoisonDamageType.h"

// Sets default values
APoisonFog::APoisonFog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Ž�� ���� ���� �� ����
    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->InitSphereRadius(150.0f); // �ʿ信 ���� ����
    //DetectionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    SetRootComponent(DetectionSphere);

    PoisonFog = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PoisonFog"));
    PoisonFog->SetupAttachment(DetectionSphere);

	Explosion = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion"));
    Explosion->SetupAttachment(DetectionSphere);

    // �⺻ �� ����
    DamagePerSecond = 10.0f; // �⺻ ���ط�
}

// Called when the game starts or when spawned
void APoisonFog::BeginPlay()
{
	Super::BeginPlay();

    DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &APoisonFog::OnBeginOverlap);
    DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &APoisonFog::OnEndOverlap);

    SetLifeSpan(5.f);
}

// Called every frame
void APoisonFog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // �������� ���͵鿡�� ���������� ���ظ� ����
    ApplyDamageToOverlappedActors(DeltaTime);
}

void APoisonFog::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && !OverlappedActors.Contains(OtherActor))
    {
        OverlappedActors.Add(OtherActor);
    }
}

void APoisonFog::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
    if (OtherActor && OverlappedActors.Contains(OtherActor))
    {
        OverlappedActors.Remove(OtherActor);
    }
}

void APoisonFog::ApplyDamageToOverlappedActors(float DeltaTime)
{
    for (AActor* Actor : OverlappedActors)
    {
        if (IsValid(Actor))
        {
            UGameplayStatics::ApplyDamage(Actor, DamagePerSecond * DeltaTime, GetInstigatorController(), nullptr, UPoisonDamageType::StaticClass());
        }
    }
}

