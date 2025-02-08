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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WBGamePlayClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WBPauseClass;

private:
	UFUNCTION()
	void ShowGamePlayWidget();
	UFUNCTION()
	void RemoveGamePlayWidget();

	UFUNCTION()
	void _Pause();

	UUserWidget* CurrentWidget;
		
};
