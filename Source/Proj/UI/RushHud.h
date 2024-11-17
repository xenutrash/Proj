// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/HUD.h"
#include "RushHud.generated.h"

class UAttributesWidget;
/**
 * 
 */
UCLASS()
class PROJ_API ARushHud : public AHUD
{
	GENERATED_BODY()

	public:
	void Init();
	
	protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributesWidget>  AttributeWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UAttributesWidget> AttributeWidget;
};
