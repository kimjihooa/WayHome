// Fill out your copyright notice in the Description page of Project Settings.


#include "WHPlayerController.h"

AWHPlayerController::AWHPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> GamePlayBP(TEXT("/Game/UI/GamePlay/WB_GamePlay.WB_GamePlay_C"));
	if (GamePlayBP.Succeeded())
	{
		WBGamePlayClass = GamePlayBP.Class;
	}

	CurrentWidget = nullptr;
}

void AWHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Tab"), EInputEvent::IE_Pressed, this, &AWHPlayerController::ShowGamePlayWidget);
	InputComponent->BindAction(TEXT("Tab"), EInputEvent::IE_Released, this, &AWHPlayerController::RemoveGamePlayWidget);
}

void AWHPlayerController::ShowGamePlayWidget()
{
	if (WBGamePlayClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, WBGamePlayClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	//Input Mode to Game and UI
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AWHPlayerController::RemoveGamePlayWidget()
{
	if (CurrentWidget && CurrentWidget->IsA(WBGamePlayClass))
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	//Input Mode to Game only
	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
