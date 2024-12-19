// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

struct FConnectedPlayer;
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

	bool bLoggedIn = false; 
public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAllPlayerModels();

	UFUNCTION(BlueprintCallable)
	void ChangePlayerCharacter(APlayerController* Controller, FName NameOfCharacter);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCharacterModel(APlayerController* Controller, FName NameOfCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnPlayer(APlayerController* Controller, FConnectedPlayer PlayerInfo); 

	UFUNCTION(BlueprintImplementableEvent)
	void RemovePlayer(APlayerController* Controller); 
	virtual void GenericPlayerInitialization(AController* Controller) override;
	
};
