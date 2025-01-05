// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "WHIntro_level.generated.h"

/**
 * 
 */
UCLASS()
class WAYHOME_API AWHIntro_level : public ALevelScriptActor
{
	GENERATED_BODY()

public:
    AWHIntro_level();

    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> WBIntroClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> WBMainMenu;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* IntroSound;

private:
    UPROPERTY()
    UUserWidget* CurrentWidget;

    void HandleSpaceBarInput();

};
