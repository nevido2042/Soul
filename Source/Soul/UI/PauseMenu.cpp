// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "HUD/SoulHUD.h"

void UPauseMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &UPauseMenu::OnContinueClicked);
    }

    if (OptionsButton)
    {
        OptionsButton->OnClicked.AddDynamic(this, &UPauseMenu::OnOptionsClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UPauseMenu::OnExitClicked);
    }
}

void UPauseMenu::OnContinueClicked()
{
    SetVisibility(ESlateVisibility::Hidden);

    APlayerController* PlayerCont = GetWorld()->GetFirstPlayerController();
    if (PlayerCont)
    {
        FInputModeGameOnly GameInputMode;
        PlayerCont->SetInputMode(GameInputMode);
        PlayerCont->bShowMouseCursor = false;
    }
}

void UPauseMenu::OnOptionsClicked()
{
    //open options
    ASoulHUD* SoulHUD = Cast<ASoulHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (SoulHUD)
    {
        SoulHUD->OpenAndClosePauseMenu();
        SoulHUD->OpenAndCloseGraphicSettings();
    }
    
}

void UPauseMenu::OnExitClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("TitleMap"));
}
