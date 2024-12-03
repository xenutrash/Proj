// Fill out your copyright notice in the Description page of Project Settings.


#include "RushGameplayAbility.h"

URushGameplayAbility::URushGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
}