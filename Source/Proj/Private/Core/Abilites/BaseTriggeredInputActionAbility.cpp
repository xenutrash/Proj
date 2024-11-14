// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilites/BaseTriggeredInputActionAbility.h"

#include "EnhancedInputComponent.h"
#include "PlayerGameplayAbilitiesDataAsset.h"
#include "RushCharacter.h"

UBaseTriggeredInputActionAbility::UBaseTriggeredInputActionAbility(const FObjectInitializer& ObjectInitializer)
 : Super(ObjectInitializer)
 , bCancelAbilityOnInputReleased(true)
{
 InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
 NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void UBaseTriggeredInputActionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
 Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

 bool bSuccess = false;

 if (const ARushCharacter* PlayerCharacter = Cast<ARushCharacter>(GetAvatarActorFromActorInfo()))
 {
  EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerCharacter->InputComponent);
  if (EnhancedInputComponent)
  {
   if (const UPlayerGameplayAbilitiesDataAsset* PlayerGameplayAbilitiesDataAsset = PlayerCharacter->GetPlayerGameplayAbilitiesDataAsset())
   {
    const TSet<FGameplayInputAbilityInfo>& InputAbilities = PlayerGameplayAbilitiesDataAsset->GetInputAbilities();
    for (const auto& It : InputAbilities)
    {
     if (It.IsValid() && It.GameplayAbilityClass == GetClass())
     {
      const FEnhancedInputActionEventBinding& TriggeredEventBinding = EnhancedInputComponent->BindAction(It.InputAction, ETriggerEvent::Triggered, this, &UBaseTriggeredInputActionAbility::OnTriggeredInputAction);
      const uint32 TriggeredEventHandle = TriggeredEventBinding.GetHandle();

      TriggeredEventHandles.AddUnique(TriggeredEventHandle);
      
      bSuccess = true;
     }
    }
   }
  }
 }

 if (bSuccess)
 {
  // You can skip committing ability here if the ability was committed in the parent class
  CommitAbility(Handle, ActorInfo, ActivationInfo);
 }
 else
 {
  constexpr bool bReplicateCancelAbility = true;
  CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
 }
}

void UBaseTriggeredInputActionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
 Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

 if (EnhancedInputComponent)
 {
  for (const uint32 EventHandle : TriggeredEventHandles)
  {
   EnhancedInputComponent->RemoveBindingByHandle(EventHandle);
  }
  
  EnhancedInputComponent = nullptr;
 }

 TriggeredEventHandles.Reset();
}

void UBaseTriggeredInputActionAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
 Super::InputReleased(Handle, ActorInfo, ActivationInfo);

 if (bCancelAbilityOnInputReleased)
 {
  CancelAbility(Handle, ActorInfo, ActivationInfo, true);
 }
}

void UBaseTriggeredInputActionAbility::OnTriggeredInputAction(const FInputActionValue& Value)
{
 
}