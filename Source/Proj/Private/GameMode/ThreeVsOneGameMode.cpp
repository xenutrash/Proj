// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ThreeVsOneGameMode.h"

#include "AFGIMain.h"
#include "Proj/MythbreakPlayerState.h"

void AThreeVsOneGameMode::OnAllPlayersConnected()
{
	for (const auto Controller : ConnectedPlayers)
	{
		const auto MythState = Controller->GetPlayerState<AMythbreakPlayerState>();

		if(MythState == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("The player does not have a MythBreak Playerstate")); 
			continue;
		}

		const FConnectedPlayer* PlayerInfo = GameInstance->GetConnectedPlayers()->Find(MythState->GetUniqueID());

		if(PlayerInfo == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("The player does not have a valid playerID")); 
			continue;
		}

		OnSpawnPlayer(Controller, *PlayerInfo, GameInstance->GetGameModeSettings()); 
	}
	UE_LOG(LogTemp, Warning, TEXT("All players spawned"));
	OnPlayersSpawned(GameInstance->GetGameModeSettings());
}

void AThreeVsOneGameMode::BeginPlay()
{
	Super::BeginPlay();
	const auto Gi = GetWorld()->GetGameInstance();
	if(Gi == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get gameInstance")); 
	}

	GameInstance = Cast<UAFGIMain>(Gi);
	if(GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid game instance")); 
	} 
	
}

void AThreeVsOneGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if(!NewPlayer->IsPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not a player controller")); 
		return; 
	}
	
	APlayerController* ConnectedPlayer = Cast<APlayerController>(NewPlayer);
	if(ConnectedPlayer == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast to a player controller")); 
		return; 
	}
	if(ConnectedPlayers.Contains(ConnectedPlayer))
	{
		// this should never be the case
		UE_LOG(LogTemp, Warning, TEXT("Player has already connected")); 
		return; 
	}

	if(ConnectedPlayers.Num() >= GameInstance->GetConnectedPlayers()->Num())
	{
		OnAllPlayersConnected();
	}
}
