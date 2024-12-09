// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "AFGIMain.generated.h"

class AMythbreakPlayerState;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FConnectedPlayer
{
	GENERATED_USTRUCT_BODY()
	FName SelectedCharacter;
	FName SelectedSkin; 
	float PlayerIndex;
	bool IsBoss;
	
	
};

USTRUCT(BlueprintType)
struct FGameModeSettings
{
	GENERATED_USTRUCT_BODY()
	float HealthMultiplier;
	
};

UCLASS()


class PROJ_API UAFGIMain : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TMap<int, FConnectedPlayer>* ConnectedPlayers;

	void TravelServer() const;
	
	UPROPERTY()
	FGameModeSettings GameModeSettings;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FName DefaultHero = TEXT("Alchemist");
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FName DefaultBoss = TEXT("Nixa");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FString LevelFilePath = "/Game/TopDown/Maps/TopDownMap";

	AMythbreakPlayerState* GetMythBreakState(const APlayerController* Controller) const; 
	
public:


	UFUNCTION(BlueprintCallable)
	void StartGame();
	
	UFUNCTION(BlueprintCallable)
	void AddNewPlayer(const APlayerController* Controller, bool isBoss = false);
	
	UFUNCTION(BlueprintCallable)
	bool RemovePlayer (const APlayerController* Controller);

	UFUNCTION(BlueprintCallable)
	void UpdateSelectedPlayer(const APlayerController* Controller, const FName NameOfCharacter);
	
	UFUNCTION(BlueprintCallable)
	void UpdateSelectedSkin(const APlayerController* Controller, const FName NameOfSkin); 
	
	TMap<int, FConnectedPlayer>* GetConnectedPlayers() const;
	
	bool SetSelectedCharacter;

	FGameModeSettings GetGameModeSettings() const; 
	
};
