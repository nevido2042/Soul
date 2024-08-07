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
        // ���콺 Ŀ���� ���̰� ����
        PlayerController->bShowMouseCursor = true;

        // �Է� ��带 UI �������� ����
        FInputModeUIOnly InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputMode);
    }
}
