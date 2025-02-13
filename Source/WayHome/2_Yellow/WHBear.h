// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../InteractionInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
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

	UPROPERTY(BlueprintReadWrite)
	float DashPower;
	UPROPERTY(BlueprintReadWrite)
	float MaxDashChargeTime;
	UPROPERTY(BlueprintReadWrite)
	float DashCooltime;
	FTimerHandle DashCooltimer;
	bool bCanDash;
	UFUNCTION(BlueprintCallable)
	void ResetDashTimer();

private:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);

	ACharacter* OriginalPawn;
	void GetOff();

	void Dash(float ChargeTime);
	void StartDashCharge();
	void EndDashCharge();
	float DashChargeStartTime;
};
