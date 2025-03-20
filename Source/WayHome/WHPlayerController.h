// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "WHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WAYHOME_API AWHPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWHPlayerController();
	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;

private:
	//Pause
	UPROPERTY()
	TSubclassOf<UUserWidget> WBPauseClass;
	UUserWidget* CurrentPauseWidget;

		
};
