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
class UInputAction;
class UInputMappingContext;
class UInputComponent;
struct FInputActionValue;
UCLASS()
class PROJ_API ARushCharacter : public AProjCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BasicAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SpecialAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UltimateAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BossAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TauntAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ConfirmAction;

public:
	ARushCharacter();
	

	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle AddPassiveEffect(const TSubclassOf<class UGameplayEffect>& Effect);

	UFUNCTION(BlueprintCallable, Category = "CppBPFLibrary")
	static void FlushMovementData(class UCharacterMovementComponent* MovComp);
	
	UFUNCTION(BlueprintCallable)
	void AddActiveAbility(const TSubclassOf<URushGameplayAbility>& Ability);

	UFUNCTION(BlueprintCallable)
	void RemoveActiveAbility();

	UFUNCTION(BlueprintCallable)
	void RemovePassiveAbility(const FActiveGameplayEffectHandle EffectHandle, const int AmountToRemove = 1) const;
	
	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void InstantDeath();
	

	
protected:

	/***********
		*Overrides 
	************/
	
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	/***********
		*Actions
	************/
	
	UInputMappingContext* MappingContext;
	void Move(const FInputActionValue& Value);
	void BasicAttack();
	void SpecialAttack();
	void UltimateAttack();
	void BossAttack();
	void Taunt();
	void Dash();
	void MovementAbility();
	void Confirm();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMove();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBasicAttack();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpecialAttack();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUltimateAttack();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBossAttack();
	UFUNCTION(BlueprintImplementableEvent)
	void OnTaunt();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDash();
	UFUNCTION(BlueprintImplementableEvent)
	void OnMovementAbility();
	
	/***********
		*Ability System Components 
	************/

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<URushAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Transient, meta=(AllowPrivateAccess=true))
	TObjectPtr<URushAttributeSet> Attributes;

	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetPlayerTags();

	
	/***********
	 Ability System stuff
	 ***********/
	
	UFUNCTION(BlueprintNativeEvent)
	void OnHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
	
	virtual void HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARushCharacter* InstigatorCharacter, AActor* DamageCauser);
	friend URushAttributeSet;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARushCharacter* InstigatorCharacter, AActor* DamageCauser );

	UFUNCTION(BlueprintCallable)
	void RefreshActor();
	
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	URushAttributeSet* GetAttributeSet() const { return Attributes; }

	UFUNCTION(BlueprintCallable, Category = "Widgets", BlueprintCosmetic)
	void CreateGameOverMenu();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widgets")
	TSubclassOf<UUserWidget> GameOverMenuWidget;
	
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* GameOverMenu;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintCosmetic)
	void DisplayGameOverWidget();

	UFUNCTION(BlueprintCallable)
	void DisplayGameOverWidgetEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttributeSetInited(); 
	
	
	
protected:
	void AddStartupGameplayAbilities();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilites")
	TArray<TSubclassOf<class UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilites")
	TArray<TSubclassOf<URushGameplayAbility>> GameplayAbilities;
	
	

	
	
	UPROPERTY()
	uint8 bAbilitiesInitialized:1;


	void SetBinds(); 

private:
	void InitHud() const;
};
