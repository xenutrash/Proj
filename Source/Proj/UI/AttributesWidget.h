// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "AttributesWidget.generated.h"


/**
 * 
 */
class UProgressBar;
UCLASS()
class PROJ_API UAttributesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindToAttributes();
	void SetHealthBarToHalf();
protected:

	// Progress bar reference (should be assigned in the widget blueprint)
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	// Sets the health bar to 50%

	
	UPROPERTY(BlueprintReadOnly)
	float HealthPercentage;

	
};
