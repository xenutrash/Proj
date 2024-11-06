// Fill out your copyright notice in the Description page of Project Settings.


#include "RushCharacter.h"

#include "Core/Abilites/RushAttributeSet.h"
#include "Proj/RushAbilitySystemComponent.h"
#include "Proj/RushGameplayAbility.h"


ARushCharacter::ARushCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<URushAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<URushAttributeSet>(TEXT("Attributes"));
}


// Called to bind functionality to input
void ARushCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	SetBinds();
}



FActiveGameplayEffectHandle ARushCharacter::AddPassiveEffect(const TSubclassOf<UGameplayEffect>& Effect)
{
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle GameplayEffectSpecHandle =
		AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContextHandle);

	if(GameplayEffectSpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle =
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), AbilitySystemComponent );
		return ActiveGameplayEffectHandle;
	}
	return NULL;
}

void ARushCharacter::AddActiveAbility(const TSubclassOf<URushGameplayAbility>& Ability) 
{
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
	Ability, 1,static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
}

void ARushCharacter::RemoveActiveAbility()
{
	//AbilitySystemComponent->RemoveActiveEffects()
}

void ARushCharacter::RemovePassiveAbility(const FActiveGameplayEffectHandle EffectHandle,  const int AmountToRemove) const
{
	const bool F = AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle, AmountToRemove);
}




FGameplayTagContainer ARushCharacter::GetPlayerTags()
{
	FGameplayTagContainer Container;
	AbilitySystemComponent->GetOwnedGameplayTags(Container);
	return Container;
}



void ARushCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
	}
}

void ARushCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	SetBinds();
}

void ARushCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);
	if( GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		//UE_LOG(LogTemp, Warning, TEXT("I have been called"))
		for( const TSubclassOf<URushGameplayAbility>& Ability : GameplayAbilities )
		{
			AddActiveAbility(Ability);
		}
		
		for( const TSubclassOf<UGameplayEffect>& Effect : PassiveGameplayEffects )
		{
			AddPassiveEffect(Effect);
		}
		bAbilitiesInitialized = true;
	
	}
}


UAbilitySystemComponent* ARushCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}



void ARushCharacter::SetBinds() const
{
	if(AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"ERivetAbilityInputID",
			static_cast<int32>(ERushAbilityInputID::Confirm),
			static_cast<int32>(ERushAbilityInputID::Cancel)
			);
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}