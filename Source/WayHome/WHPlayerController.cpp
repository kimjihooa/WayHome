// Fill out your copyright notice in the Description page of Project Settings.


#include "WHPlayerController.h"

AWHPlayerController::AWHPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> GamePlayBP(TEXT("/Game/UI/GamePlay/WB_GamePlay.WB_GamePlay_C"));
	if (GamePlayBP.Succeeded())
	{
		WBGamePlayClass = GamePlayBP.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> PauseBP(TEXT("/Game/UI/GamePlayPausepopup/WB_GamePlayPausePopUp.WB_GamePlayPausePopUp_C"));
	if (PauseBP.Succeeded())
	{
		WBPauseClass = PauseBP.Class;
	}
	CurrentWidget = nullptr;
}

void AWHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Tab"), EInputEvent::IE_Pressed, this, &AWHPlayerController::ShowGamePlayWidget);
	InputComponent->BindAction(TEXT("Tab"), EInputEvent::IE_Released, this, &AWHPlayerController::RemoveGamePlayWidget);
	//InputComponent->BindAction(TEXT("Esc"), EInputEvent::IE_Pressed, this, &AWHPlayerController::_Pause);
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

void AWHPlayerController::_Pause()
{
	if (WBPauseClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, WBPauseClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			//CurrentWidget->SetKeyboardFocus();
		}

		//FInputModeGameAndUI InputMode;
		//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		//SetInputMode(InputMode);
		//bShowMouseCursor = true;
		//SetPause(true);
	}
}
