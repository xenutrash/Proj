// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"

#include "AFGIMain.generated.h"

class FContainerTestClass;
class AMythbreakPlayerState;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FConnectedPlayer
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadOnly)
	FName SelectedCharacter = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FName SelectedSkin = TEXT("");;
	UPROPERTY(BlueprintReadOnly)
	mutable float PlayerIndex = 0;
	UPROPERTY(BlueprintReadOnly)
	bool IsBoss = false;
};

USTRUCT(BlueprintType)
struct FGameModeSettings
{
	GENERATED_USTRUCT_BODY()
	float HealthMultiplier = 1;
	
};

UCLASS()


class PROJ_API UAFGIMain : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

private:

	void TravelServer(const FString Path) const;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	FGameModeSettings GameModeSettings;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FName DefaultHero = TEXT("Alchemist");
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FName DefaultBoss = TEXT("Nixa");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FString DefaultMapLevelFilePath = "/Game/TopDown/Maps/TopDownMap";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FString LobbyMapFilePath = "/Game/Programming/Lobby/LobbyMenuMap";

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FString UserSelectedMap = "/Game/TopDown/Maps/TopDownMap";
	
	AMythbreakPlayerState* GetMythBreakState(const APlayerController* Controller) const; 
	virtual void StartGameInstance() override;
	
	TMap<FUniqueNetIdRepl , FConnectedPlayer> ConnectedPlayers;
	void PrintAllUserIds() const;


	
public:

	UFUNCTION(BlueprintCallable)
	FConnectedPlayer GetPlayerInfo (APlayerController* Controller) const; 

	UFUNCTION(BlueprintCallable)
	void TravelToLobbyMap();
	
	UAFGIMain(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void StartGame();
	
	UFUNCTION(BlueprintCallable)
	void AddNewPlayer(const APlayerController* Controller, const bool IsBoss = false);
	
	UFUNCTION(BlueprintCallable)
	bool RemovePlayer (const APlayerController* Controller);

	UFUNCTION(BlueprintCallable)
	void UpdateSelectedPlayer(const APlayerController* Controller, const FName NameOfCharacter);
	
	UFUNCTION(BlueprintCallable)
	void UpdateSelectedSkin(const APlayerController* Controller, const FName NameOfSkin); 
	
	const TMap<FUniqueNetIdRepl, FConnectedPlayer>* GetConnectedPlayers() const;
	
	FGameModeSettings GetGameModeSettings() const;

	UFUNCTION(BlueprintCallable)
	void CreateLanServer();
	UFUNCTION(BlueprintCallable)
	void CreateOnlineServer();

	UFUNCTION(BlueprintImplementableEvent)
	void OnServerCreated();

	UFUNCTION(BlueprintCallable)
	void ResetActivePlayers();

	UFUNCTION(BlueprintCallable)
	void SetUserSelectMap(FString SelectedMap);
	
};
