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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGimmikInteract);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;
	UPROPERTY(BlueprintReadWrite)
	float CrouchSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanSprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanCrouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAscend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDescend;

	//Interaction
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* Interaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* InteractableActor;
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnGimmikInteract OnGimmikInteract;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* UIToInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* AsceInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* DescInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* GimmInputAction;

private:
	//Movements
	//void MoveForward(float AxisValue);
	//void MoveRight(float NewAxisValue);
	//void Turn(float NewAxisValue);
	//void LookUp(float NewAxisValue);
	void Walk();
	void Sprint();
	void Crouch_();
	void Jump_();
	//New Input System
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Ascend(const FInputActionValue& Value);
	void Descend(const FInputActionValue& Value);

	//Interaction
	IInteractionInterface* Interface;
	void Interact();
	void Gimmik();

protected:
	//Abilities
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	UAbilitySystemComponent* GetAbilityComponent();
	void DashCharge();
	void Dash();
	void DoubleJump();
	virtual void Landed(const FHitResult& Hit) override;
	void Glide();
	void EndGlide();
};
