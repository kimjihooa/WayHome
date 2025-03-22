// Fill out your copyright notice in the Description page of Project Settings.


#include "GADash.h"

UGADash::UGADash()
{
	FGameplayTag AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Dash"));
	AbilityTags.AddTag(AbilityTag);
}

void UGADash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//UE_LOG(LogTemp, Warning, TEXT("Dash"));
}