// Fill out your copyright notice in the Description page of Project Settings.


#include "WHIntro_level.h"

AWHIntro_level::AWHIntro_level()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WBIntroBP(TEXT("/Game/UI/Intro/WB_Intro.WB_Intro_C"));
	if (WBIntroBP.Succeeded())
	{
		WBIntroClass = WBIntroBP.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBP(TEXT("/Game/UI/Mainmenu/WB_MainMenu.WB_MainMenu_C"));
	if (MainMenuBP.Succeeded())
	{
		WBMainMenu = MainMenuBP.Class;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> IntroSoundAsset(TEXT("/Game/Assets/Sound/S_mysterious.S_mysterious"));
	if (IntroSoundAsset.Succeeded())
	{
		IntroSound = IntroSoundAsset.Object;
	}
	CurrentWidget = nullptr;
}

void AWHIntro_level::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (controller)
		controller->SetShowMouseCursor(false);

	if (WBIntroClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(controller, WBIntroClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	if (IntroSound)
		UGameplayStatics::PlaySound2D(this, IntroSound);

	if (controller)
	{
		if (!controller->InputComponent)
		{
			controller->InputComponent = NewObject<UInputComponent>(controller);
			controller->InputComponent->RegisterComponent();
		}
		controller->InputComponent->BindAction("Jump", IE_Pressed, this, &AWHIntro_level::HandleSpaceBarInput);
	}
}

void AWHIntro_level::HandleSpaceBarInput()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (WBMainMenu && PlayerController)
	{
		CurrentWidget = CreateWidget<UUserWidget>(PlayerController, WBMainMenu);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
		}
	}
}
