// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
class UButton;

UCLASS()
class SOUL_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* ContinueButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* OptionsButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* ExitButton;

private:

    UFUNCTION()
    void OnContinueClicked();

    UFUNCTION()
    void OnOptionsClicked();

    UFUNCTION()
    void OnExitClicked();
};
