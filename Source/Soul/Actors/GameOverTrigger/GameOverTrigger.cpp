// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GameOverTrigger/GameOverTrigger.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameOverTrigger::AGameOverTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(100.0f);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AGameOverTrigger::OnOverlapBegin);
	SetRootComponent(SphereComponent);
}

// Called when the game starts or when spawned
void AGameOverTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void AGameOverTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::OpenLevel(this, TEXT("TitleMap"));
}


