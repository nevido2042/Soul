// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SoulHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/StatusWidget.h"
#include "Actors/PlayerState/SoulPlayerState.h"
#include "Components/HealthComponent.h"

void ASoulHUD::BeginPlay()
{
    Super::BeginPlay();

    if (StatusWidgetAsset)
    {
        StatusWidget = CreateWidget<UStatusWidget>(GetWorld(), StatusWidgetAsset);

        if (StatusWidget)
        {
            StatusWidget->AddToViewport();
        }
    }

    APlayerController* PlayerController = GetOwningPlayerController();
    if (PlayerController)
    {
        PlayerState = Cast<ASoulPlayerState>(PlayerController->PlayerState);
    }

    UpdateStatusWidget();
}

void ASoulHUD::UpdateStatusWidget()
{
    float InHealth = PlayerState->GetHealthComponent()->CurrentHealth / PlayerState->GetHealthComponent()->DefaultHealth;
    StatusWidget->SetHealthBar(InHealth);
}
