// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASStructs.h"
#include "Abilities/GameplayAbility.h"
#include "RushGameplayAbility.generated.h"

enum class ERushAbilityInputID : uint8;
/**
 * 
 */
UCLASS()
class PROJ_API URushGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


public:
	URushGameplayAbility(); 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Abilities")
	ERushAbilityInputID AbilityInputID{ERushAbilityInputID::None};
	
	
};
