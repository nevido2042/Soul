// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UTitle::NativeConstruct()
{
	Super::NativeConstruct();

	if (GameStartBtn)
	{
		GameStartBtn->OnClicked.AddDynamic(this, &UTitle::OnGameStartBtnClicked);
	}

	if (ExitBtn)
	{
		ExitBtn->OnClicked.AddDynamic(this, &UTitle::OnExitClicked);
	}
}

void UTitle::OnGameStartBtnClicked()
{
	UGameplayStatics::OpenLevel(this, TEXT("DungeonMap"));
}

void UTitle::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
