// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "InputAction.h"
#include "WHGAChargeDash.generated.h"

/**
 * 
 */
UCLASS()
class WAYHOME_API UWHGAChargeDash : public UGameplayAbility
{
	GENERATED_BODY()
	
    UWHGAChargeDash();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

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
};
