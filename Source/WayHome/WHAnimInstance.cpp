// Fill out your copyright notice in the Description page of Project Settings.


#include "WHAnimInstance.h"

UWHAnimInstance::UWHAnimInstance()
{
	CurrentSpeed = 0.0f;
}

void UWHAnimInstance::NativeInitializeAnimation()
{
	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		Character = Cast<ACharacter>(GetOwningActor());
		if (Character)
		{
			Movement = Character->GetCharacterMovement();
		}
	}
}

void UWHAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(Character))
		return;

	FVector Velocity = Movement->Velocity;
	CurrentSpeed = FVector(Velocity.X, Velocity.Y, 0).Size();
	
	bShouldMove = (CurrentSpeed > 3.0f) && (Movement->GetCurrentAcceleration() != FVector::ZeroVector);
}