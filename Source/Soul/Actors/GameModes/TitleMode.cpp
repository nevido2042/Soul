// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GameModes/TitleMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ATitleMode::BeginPlay()
{
	if (TitleAsset)
	{
		Title = CreateWidget(GetWorld(), TitleAsset);
		Title->AddToViewport();
	}

    EnableUIMode();
}

void ATitleMode::EnableUIMode()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        // 마우스 커서를 보이게 설정
        PlayerController->bShowMouseCursor = true;

        // 입력 모드를 UI 전용으로 설정
        FInputModeUIOnly InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputMode);
    }
}
