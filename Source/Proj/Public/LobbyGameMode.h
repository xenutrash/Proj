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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="Core")
	int32 NumbersOfNoneSpectators = 4; 
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY()
	UAFGIMain* GameInstance;
	virtual void Logout(AController* Exiting) override;

	bool bLoggedIn = false;
	UAFGIMain* GetGameInstance();
	
	UPROPERTY()
	TMap<const APlayerController* , bool> ReadyList;
public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAllPlayerModels();

	UFUNCTION(BlueprintCallable)
	void ChangePlayerCharacter(APlayerController* Controller, FName NameOfCharacter);
	
	UFUNCTION(BlueprintCallable)
	void SetPLayerAsSpectator(APlayerController* Controller); 
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCharacterModel(APlayerController* Controller, FName NameOfCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnPlayer(APlayerController* Controller, FConnectedPlayer PlayerInfo); 

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnPlayerAsSpectator(APlayerController* Controller); 
	
	UFUNCTION(BlueprintImplementableEvent)
	void RemovePlayer(APlayerController* Controller);
	
	virtual void GenericPlayerInitialization(AController* Controller) override;

	UFUNCTION(BlueprintCallable)
	void SetReadyStateForPlayer(const APlayerController* PlayerController, const bool IsReady);
	UFUNCTION(BlueprintCallable)
	bool GetReadyStateForPlayer(const APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool ArePlayersReady() const;
	UFUNCTION(BlueprintImplementableEvent)
	void OnReadyStateChanged(const APlayerController* Controller, const bool IsReady);
	UFUNCTION(BlueprintCallable)
	void RemoveReadyState(const APlayerController* Controller);
	

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool MaxPlayersReached() const; 
	
	
};
