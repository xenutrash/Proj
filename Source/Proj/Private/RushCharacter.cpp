// Fill out your copyright notice in the Description page of Project Settings.


#include "RushCharacter.h"

#include "Core/Abilites/RushAttributeSet.h"
#include "Proj/RushAbilitySystemComponent.h"
#include "Proj/RushGameplayAbility.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Proj/ProjPlayerController.h"
#include "InputActionValue.h"

ARushCharacter::ARushCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bAbilitiesInitialized = false; 

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
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARushCharacter::Move);
		EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &ARushCharacter::BasicAttack);
		EnhancedInputComponent->BindAction(SpecialAttackAction, ETriggerEvent::Triggered, this, &ARushCharacter::SpecialAttack);
		EnhancedInputComponent->BindAction(UltimateAttackAction, ETriggerEvent::Triggered, this, &ARushCharacter::UltimateAttack);
		EnhancedInputComponent->BindAction(BossAttackAction, ETriggerEvent::Triggered, this, &ARushCharacter::BossAttack);
		EnhancedInputComponent->BindAction(TauntAction, ETriggerEvent::Triggered, this, &ARushCharacter::Taunt);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ARushCharacter::Dash);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	
}

void ARushCharacter::Move(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Move"));
}

void ARushCharacter::BasicAttack()
{
	UE_LOG(LogTemp, Display, TEXT("BasicAttack"));
}

void ARushCharacter::SpecialAttack()
{
	UE_LOG(LogTemp, Display, TEXT("SpecialAttack"));
}

void ARushCharacter::UltimateAttack()
{
	UE_LOG(LogTemp, Display, TEXT("UltimateAttack"));
}

void ARushCharacter::BossAttack()
{
	UE_LOG(LogTemp, Display, TEXT("BossAttack"));
}

void ARushCharacter::Taunt()
{
	UE_LOG(LogTemp, Display, TEXT("Taunt"));
}

void ARushCharacter::Dash()
{
	UE_LOG(LogTemp, Display, TEXT("Dash"));
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

void ARushCharacter::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if(bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void ARushCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, ARushCharacter* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser); 
}


void ARushCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// server gas init
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
		// grants abilities on the server 
		
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



void ARushCharacter::SetBinds() 
{
	if(AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"ERushAbilityInputID",
			static_cast<int32>(ERushAbilityInputID::Confirm),
			static_cast<int32>(ERushAbilityInputID::Cancel)
			);
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}

	AddStartupGameplayAbilities();
}