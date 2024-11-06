// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ERushAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	BasicAttack,
	SpecialAttack,
	UltimateAttack,
	BuffAbility,
	BossAttack
};


