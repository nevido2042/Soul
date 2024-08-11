// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SoulHUD.h"
#include "Blueprint/UserWidget.h"

//void ASoulHUD::DrawHUD()
//{
//    Super::DrawHUD();
//
//    if (HealthWidgetClass)
//    {
//        HealthWidget = CreateWidget<UUserWidget>(GetWorld(), HealthWidgetClass);
//
//        if (HealthWidget)
//        {
//            HealthWidget->AddToViewport();
//        }
//    }
//}

void ASoulHUD::BeginPlay()
{
    Super::BeginPlay();

    if (HealthWidgetClass)
    {
        HealthWidget = CreateWidget<UUserWidget>(GetWorld(), HealthWidgetClass);

        if (HealthWidget)
        {
            HealthWidget->AddToViewport();
        }
    }
}
