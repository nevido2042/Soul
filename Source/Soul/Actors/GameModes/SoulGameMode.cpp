// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GameModes/SoulGameMode.h"

void ASoulGameMode::BeginPlay()
{
    HideMouseAndSetGameMode();
}

void ASoulGameMode::HideMouseAndSetGameMode()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // 마우스 커서 숨기기
        PlayerController->bShowMouseCursor = false;

        // 입력 모드를 게임 모드로 변경
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
    }
}