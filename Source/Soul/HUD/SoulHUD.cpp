// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SoulHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/StatusWidget.h"
#include "Actors/PlayerState/SoulPlayerState.h"
#include "Components/HealthComponent.h"
#include "UI/GraphicsSettings.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/YouDiedWidget.h"
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

    if (PauseMenuWidgetAsset)
    {
        PauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetAsset);

        if (PauseMenuWidget)
        {
            PauseMenuWidget->AddToViewport();
            PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (GraphicSettingsWidgetAsset)
    {
        GraphicSettingsWidget = CreateWidget<UGraphicsSettings>(GetWorld(), GraphicSettingsWidgetAsset);

        if (GraphicSettingsWidget)
        {
            GraphicSettingsWidget->AddToViewport();
            GraphicSettingsWidget->SetVisibility(ESlateVisibility::Hidden);
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
    // 1. �÷��̾� ���¿��� ü�� ������Ʈ�� ������
    UHealthComponent* HealthComponent = PlayerState->GetHealthComponent();

    // 2. ���� ü�°� �⺻ ü���� ������
    float CurrentHealth = HealthComponent->CurrentHealth;
    float DefaultHealth = HealthComponent->DefaultHealth;

    // 3. ü�� ������ �����
    float HealthRatio = CurrentHealth / DefaultHealth;

    // 4. UI�� ü�� �ٸ� ������Ʈ��
    StatusWidget->SetHealthBar(HealthRatio);
}

void ASoulHUD::OpenAndClosePauseMenu()
{
    if (PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    else if (PauseMenuWidget->GetVisibility() == ESlateVisibility::Hidden)
    {
        PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);

        APlayerController* PlayerCont = GetWorld()->GetFirstPlayerController();
        if (PlayerCont)
        {
            FInputModeUIOnly InputMode;
            PlayerCont->SetInputMode(InputMode);
            PlayerCont->bShowMouseCursor = true;
        }
    }
}

void ASoulHUD::OpenAndCloseGraphicSettings()
{
    if (GraphicSettingsWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        GraphicSettingsWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    else if (GraphicSettingsWidget->GetVisibility() == ESlateVisibility::Hidden)
    {
        GraphicSettingsWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void ASoulHUD::ShowYouDied()
{
    UYouDiedWidget* YouDiedWidget = CreateWidget<UYouDiedWidget>(GetWorld(), YouDiedWidgetAsset);
    if (YouDiedWidget)
    {
        YouDiedWidget->AddToViewport();

        YouDiedWidget->PlayFadeInOutAnimation();
    }
}
