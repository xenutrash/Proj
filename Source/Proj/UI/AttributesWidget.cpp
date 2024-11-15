// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesWidget.h"
#include "AttributeSet.h"
#include "RushCharacter.h"
#include "Core/Abilites/RushAttributeSet.h"

void UAttributesWidget::BindToAttributes()
{
	if (ARushCharacter* RushCharacter = Cast<ARushCharacter>(GetOwningPlayerPawn()))
	{
		// Set the health bar to 50% initially
		SetHealthBarToHalf();
	}
}

void UAttributesWidget::SetHealthBarToHalf()
{
	// Set HealthPercentage to 50% (0.5)
	HealthPercentage = 0.5f;

	// Update the progress bar to show 50%
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(HealthPercentage);
	}

	// Log to confirm the function is being called
	UE_LOG(LogTemp, Warning, TEXT("Health Progress Bar Set to 50%%"));
}