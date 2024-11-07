// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "RushAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class PROJ_API URushAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category= "Attributes")
	FGameplayAttributeData Health = 0;
	ATTRIBUTE_ACCESSORS(URushAttributeSet, Health)
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category= "Attributes")
	FGameplayAttributeData MaxHealth = 0;
	ATTRIBUTE_ACCESSORS(URushAttributeSet, MaxHealth)
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armour, Category= "Attributes")
	FGameplayAttributeData Armour = 0;
	ATTRIBUTE_ACCESSORS(URushAttributeSet, Armour)
	UFUNCTION()
	virtual void OnRep_Armour(const FGameplayAttributeData& OldValue);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage, Category= "Attributes")
	FGameplayAttributeData Damage = 10;
	ATTRIBUTE_ACCESSORS(URushAttributeSet, Damage)
	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Speed, Category= "Attributes")
	FGameplayAttributeData Speed = 700;
	ATTRIBUTE_ACCESSORS(URushAttributeSet, Speed)
	UFUNCTION()
	virtual void OnRep_Speed(const FGameplayAttributeData& OldValue);


	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const;

	
};
