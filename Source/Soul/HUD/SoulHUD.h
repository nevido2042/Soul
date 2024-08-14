// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SoulHUD.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API ASoulHUD : public AHUD
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> StatusWidgetAsset;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> PauseMenuWidgetAsset;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> GraphicSettingsWidgetAsset;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> YouDiedWidgetAsset;

private:
    class UStatusWidget* StatusWidget;

    class UGraphicsSettings* GraphicSettingsWidget;

    UUserWidget* PauseMenuWidget;

    class ASoulPlayerState* PlayerState;

public:
    void UpdateStatusWidget();

    void OpenAndClosePauseMenu();

    void OpenAndCloseGraphicSettings();

    void ShowYouDied();
};
