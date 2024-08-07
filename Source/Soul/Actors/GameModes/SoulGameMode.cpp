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
        // ���콺 Ŀ�� �����
        PlayerController->bShowMouseCursor = false;

        // �Է� ��带 ���� ���� ����
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
    }
}