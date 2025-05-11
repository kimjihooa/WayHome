// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../InteractionInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"
#include "AbilitySystemComponent.h"
#include "WHBear.generated.h"

UCLASS()
class WAYHOME_API AWHBear : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWHBear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void InteractWith_Implementation() override;
	virtual void InRange_Implementation() override;
	virtual void OutRange_Implementation() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Attach;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Detach;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* CharacterMesh;

	//Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* MoveInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* LookInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* JumpInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* InteInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* DashInputAction;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	UAbilitySystemComponent* GetAbilityComponent();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

private:
	//void MoveForward(float NewAxisValue);
	//void MoveRight(float NewAxisValue);
	//void Turn(float NewAxisValue);
	//void LookUp(float NewAxisValue);
	//New Input System
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	ACharacter* OriginalPawn;
	void GetOff();

	void DashCharge();
	void Dash();
	//void StartDashCharge();
	//void EndDashCharge();
	//float DashChargeStartTime;
};
