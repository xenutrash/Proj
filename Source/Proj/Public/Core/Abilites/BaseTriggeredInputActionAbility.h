// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseTriggeredInputActionAbility.generated.h"

struct FInputActionValue;
/**
 * 
 */
UCLASS()
class PROJ_API UBaseTriggeredInputActionAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta=(AllowPrivateAccess = "true"))
	bool bCancelAbilityOnInputReleased;

protected:
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;

	TArray<uint32> TriggeredEventHandles;

	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
public:
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~ End UGameplayAbility Interface

protected:
	//~ Begin UGameplayAbility_BaseTriggeredInputActionAbility Interface
	virtual void OnTriggeredInputAction(const FInputActionValue& Value);
	//~ End UGameplayAbility_BaseTriggeredInputActionAbility Interface	
};
