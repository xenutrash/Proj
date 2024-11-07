// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilites/DashGameplayAbility.h"

#include "RushCharacter.h"

void UDashGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	

	if(Character == nullptr){
		Character = Cast<ARushCharacter>(ActorInfo->AvatarActor);
	}
	
	if(Character)
	{
		Dash();
	}
}

void UDashGameplayAbility::ActivateAbility_Implementation()
{
	ActivateAbility();
}

void UDashGameplayAbility::Dash()
{
	//const FVector ForwardDirection = Character->GetActorRotation().Vector();
	FVector MoveDirection = Character->GetVelocity();
	MoveDirection.Z = 0.0f;
	MoveDirection = MoveDirection.GetSafeNormal();
	Character->LaunchCharacter(MoveDirection * DashDistance, true, false);

	
}