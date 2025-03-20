// Fill out your copyright notice in the Description page of Project Settings.


#include "WHPlayerController.h"

AWHPlayerController::AWHPlayerController()
{
	//Pause
	static ConstructorHelpers::FClassFinder<UUserWidget> PauseBP(TEXT("/Game/UI/GamePlayPausepopup/WB_GamePlayPausePopUp.WB_GamePlayPausePopUp_C"));
	if (PauseBP.Succeeded())
	{
		WBPauseClass = PauseBP.Class;
	}
}

void AWHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}


void AWHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WBPauseClass)
	{
		CurrentPauseWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), WBPauseClass);
		if (CurrentPauseWidget)
		{
			CurrentPauseWidget->AddToViewport();
			CurrentPauseWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}