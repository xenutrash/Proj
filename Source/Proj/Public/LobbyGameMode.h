// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

class UAFGIMain;
/**
 * 
 */
UCLASS()
class PROJ_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY()
	UAFGIMain* GameInstance;
	virtual void Logout(AController* Exiting) override;

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAllPlayerModels(); 
	
};
