// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RushGameplayAbility.h"
#include "DashGameplayAbility.generated.h"

class ARushCharacter;
/**
 * 
 */
UCLASS()
class PROJ_API UDashGameplayAbility : public URushGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
	UFUNCTION(BlueprintNativeEvent)
	void ActivateAbility();
private:
	UPROPERTY(EditAnywhere, Category = "Ability")
	float DashDistance;
	UPROPERTY()
	ARushCharacter* Character; 

	void Dash();	


	
};
