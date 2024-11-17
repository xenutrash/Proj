// Fill out your copyright notice in the Description page of Project Settings.


#include "RushHud.h"
#include "AttributesWidget.h"
#include "Blueprint/UserWidget.h"

void ARushHud::Init()
{
	 AttributeWidget = CreateWidget<UAttributesWidget>(GetOwningPlayerController(), AttributeWidgetClass);
	 AttributeWidget->BindToAttributes();
	 AttributeWidget->AddToViewport();
	AttributeWidget->SetHealthBarToHalf();
}
