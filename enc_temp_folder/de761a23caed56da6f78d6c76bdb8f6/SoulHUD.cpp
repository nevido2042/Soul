// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SoulHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/StatusWidget.h"

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

    StatusWidget->SetHealthBar(1.f);
}
