// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "../InteractionInterface.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "WHCharacter.generated.h"

UCLASS()
class WAYHOME_API AWHCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWHCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, Category = Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadWrite, Category = Camera)
	UCameraComponent* Camera;

	//Movements
	UPROPERTY(VisibleAnywhere)
	bool bIsSprinting;
	UPROPERTY(VisibleAnywhere)
	bool bIsCrouching;

	UPROPERTY(BlueprintReadWrite)
	float WalkSpeed;
	UPROPERTY(BlueprintReadWrite)
	float SprintSpeed;
	UPROPERTY(BlueprintReadWrite)
	float CrouchSpeed;

	//Interaction
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* Interaction;

	//Input System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputMappingContext* CharacterMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* MoveInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* LookInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* JumpInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* SpriInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* CrouInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* InteInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* PausInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* DashInputAction;

private:
	//Movements
	//void MoveForward(float AxisValue);
	//void MoveRight(float NewAxisValue);
	//void Turn(float NewAxisValue);
	//void LookUp(float NewAxisValue);
	void Walk();
	void Sprint();
	void Crouch_();
	//New Input System
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	//Pause
	UPROPERTY()
	TSubclassOf<UUserWidget> WBPauseClass;
	UUserWidget* CurrentPauseWidget;

	//Interaction
	IInteractionInterface* Interface;
	void Interact();
	AActor* InteractableActor;

protected:
	//Abilities
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	UAbilitySystemComponent* GetAbilityComponent();
	void DashCharge();
	void Dash();
};
