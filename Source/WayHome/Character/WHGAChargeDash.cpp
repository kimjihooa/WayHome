// Fill out your copyright notice in the Description page of Project Settings.


#include "WHGAChargeDash.h"

UWHGAChargeDash::UWHGAChargeDash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	bCanDash = true;
}

void UWHGAChargeDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
}
void UWHGAChargeDash::ResetDashTimer()
{
	bCanDash = true;
}