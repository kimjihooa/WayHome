// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	bool bIsSprinting;
	UPROPERTY(VisibleAnywhere)
	bool bIsCrouching;

	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	UPROPERTY(EditAnywhere)
	float SprintSpeed;
	UPROPERTY(EditAnywhere)
	float CrouchSpeed;

private:
	//Movements
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Walk();
	void Sprint();
	void Crouch_();
};
