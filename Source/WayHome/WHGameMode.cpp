// Fill out your copyright notice in the Description page of Project Settings.


#include "WHGameMode.h"
#include "WHCharacter.h"
#include "WHPlayerController.h"

AWHGameMode::AWHGameMode()
{
	DefaultPawnClass = AWHCharacter::StaticClass();
	PlayerControllerClass = AWHPlayerController::StaticClass();
}