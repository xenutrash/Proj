// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Proj/ProjCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "RushCharacter.generated.h"


/**
 * 
 */

struct FActiveGameplayEffectHandle;
class URushGameplayAbility;
class URushAttributeSet;
class URushAbilitySystemComponent; 
UCLASS()
class PROJ_API ARushCharacter : public AProjCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARushCharacter();


	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle AddPassiveEffect(const TSubclassOf<class UGameplayEffect>& Effect);
	
	UFUNCTION(BlueprintCallable)
	void AddActiveAbility(const TSubclassOf<URushGameplayAbility>& Ability);

	UFUNCTION(BlueprintCallable)
	void RemoveActiveAbility();

	UFUNCTION(BlueprintCallable)
	void RemovePassiveAbility(const FActiveGameplayEffectHandle EffectHandle, const int AmountToRemove = 1) const;

	
protected:



	/***********
		*Overrides 
	************/
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	

	/**********************
		Ability System Components 
		****************/
	TObjectPtr<URushAbilitySystemComponent> AbilitySystemComponent;
	TObjectPtr<URushAttributeSet> Attributes;

	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetPlayerTags();


	
	/**********************
	 Ability System stuff
	 ****************/
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
	
	virtual void HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARushCharacter* InstigatorCharacter, AActor* DamageCauser);
	friend URushAttributeSet;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARushCharacter* InstigatorCharacter, AActor* DamageCauser );
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void AddStartupGameplayAbilities();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilites")
	TArray<TSubclassOf<class UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilites")
	TArray<TSubclassOf<URushGameplayAbility>> GameplayAbilities;
	
	

	
	
	UPROPERTY()
	uint8 bAbilitiesInitialized:1;


	void SetBinds(); 
	
};
