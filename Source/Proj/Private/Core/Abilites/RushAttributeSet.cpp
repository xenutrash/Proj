// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilites/RushAttributeSet.h"

#include "Net/UnrealNetwork.h"

void URushAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URushAttributeSet, Health);
	DOREPLIFETIME(URushAttributeSet, Damage);
	DOREPLIFETIME(URushAttributeSet, Speed);
	DOREPLIFETIME(URushAttributeSet, Armour);
	DOREPLIFETIME(URushAttributeSet, MaxHealth);

}

void URushAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	/**	if(Attribute == GetMaxHealthAttribute())
	{
		//AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}**/
}

void URushAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	return;
	UE_LOG(LogTemp, Warning, TEXT("Effect has happend"))
	Super::PostGameplayEffectExecute(Data);
	const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	//UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	
	float DeltaValue {0.f};

	if(Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	ABaseCharacter* TargetCharacter {nullptr};

	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ABaseCharacter>(TargetActor);
	}

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		if(TargetCharacter)
		{
			//TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		}
	}
	
}

void URushAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URushAttributeSet, Health, OldValue);
}

void URushAttributeSet::OnRep_Armour(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URushAttributeSet, Armour, OldValue);
}

void URushAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URushAttributeSet, MaxHealth, OldValue);
}

void URushAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URushAttributeSet, Damage, OldValue);
}

void URushAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URushAttributeSet, Speed, OldValue);
}


void URushAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
                                                     const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
                                                     const FGameplayAttribute& AffectedAttributeProperty) const
{
	return;
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if(!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue/ CurrentMaxValue)-CurrentValue : NewMaxValue;
		AbilityComp-> ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
		
	}
	
}