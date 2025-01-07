// Fill out your copyright notice in the Description page of Project Settings.


#include "WHRocket.h"

// Sets default values
AWHRocket::AWHRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Rocket(TEXT("/Game/Assets/1_Gray/Rocket/SM_Rocket.SM_Rocket"));
	if (Rocket.Succeeded())
	{
		Mesh->SetStaticMesh(Rocket.Object);
	}
	Mesh->SetCollisionProfileName(TEXT("InteractableBlock"));

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	InteractionWidget->SetupAttachment(RootComponent);
	static ConstructorHelpers::FClassFinder<UUserWidget> Widget(TEXT("/Game/UI/StageSelect/Widget_PressF.Widget_PressF_C"));
	if (Widget.Succeeded())
	{
		InteractionWidget->SetWidgetClass(Widget.Class);
	}
	InteractionWidget->SetVisibility(false);
	InteractionWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	InteractionWidget->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	
	static ConstructorHelpers::FClassFinder<UUserWidget> MapBP(TEXT("/Game/UI/StageSelect/Widget_StageSelect.Widget_StageSelect_C"));
	if (MapBP.Succeeded())
	{
		WBMapClass = MapBP.Class;
	}
	CurrentWidget = nullptr;
	Controller = nullptr;
}

// Called when the game starts or when spawned
void AWHRocket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWHRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWHRocket::InteractWith()
{
	UE_LOG(LogTemp, Warning, TEXT("ROcket"));
	if (WBMapClass)
	{
		Controller = GetWorld()->GetFirstPlayerController();
		CurrentWidget = CreateWidget<UUserWidget>(Controller, WBMapClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}

		//Input Mode To UI only
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		Controller->SetInputMode(InputMode);
		Controller->bShowMouseCursor = true;
	}
}

void AWHRocket::InRange()
{
	InteractionWidget->SetVisibility(true);
}

void AWHRocket::OutRange()
{
	InteractionWidget->SetVisibility(false);
}

