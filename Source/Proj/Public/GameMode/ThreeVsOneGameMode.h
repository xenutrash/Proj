// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AFGIMain.h"
#include "GameFramework/GameModeBase.h"
#include "ThreeVsOneGameMode.generated.h"

struct FConnectedPlayer;
struct FGameModeSettings;
class UAFGIMain;
/**
 * 
 */
UCLASS()
class PROJ_API AThreeVsOneGameMode : public AGameModeBase
{
	GENERATED_BODY()
	bool bGameStarted = false; 
public:

	UFUNCTION()
	void OnAllPlayersConnected();
	virtual void BeginPlay() override;
	UPROPERTY()
	UAFGIMain* GameInstance;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawnPlayer(APlayerController* Controller, const FConnectedPlayer& PlayerInfo, const FGameModeSettings& GameModeSettings);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayersSpawned(const FGameModeSettings& GameModeSettings); 

	
	UPROPERTY()
	TArray<APlayerController*> ConnectedPlayers; 

private: 
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	
};
