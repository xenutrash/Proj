// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ThreeVsOneGameMode.h"

#include "AFGIMain.h"
#include "Proj/MythbreakPlayerState.h"


void AThreeVsOneGameMode::OnAllPlayersConnected()
{
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UAFGIMain>(GetGameInstance()); 
	}
	
	bGameStarted = true;
	int Index = 0; 
	for (const auto Controller : ConnectedPlayers)
	{
		const auto MythState = Controller->GetPlayerState<AMythbreakPlayerState>();

		if(MythState == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("The player does not have a MythBreak Playerstate")); 
			continue;
		}
		
		const FConnectedPlayer* PlayerInfo = GameInstance->GetConnectedPlayers()->Find(MythState->GetUniqueId());

		if(PlayerInfo == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("The player does not have a valid playerID")); 
			continue;
		}
		
		PlayerInfo->PlayerIndex = Index;
		
		UE_LOG(LogTemp, Log, TEXT("Spawning player with ID %i as %s "), Controller->GetUniqueID(), *PlayerInfo->SelectedCharacter.ToString() );
		OnSpawnPlayer(Controller, *PlayerInfo, GameInstance->GetGameModeSettings());
		Index++; 
	}
	UE_LOG(LogTemp, Warning, TEXT("All players spawned"));
	OnPlayersSpawned(GameInstance->GetGameModeSettings());
}

void AThreeVsOneGameMode::BeginPlay()
{
	Super::BeginPlay();
	const auto Gi = GetGameInstance();
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

void AThreeVsOneGameMode::GenericPlayerInitialization(AController* Controller)
{
	Super::GenericPlayerInitialization(Controller);

	if(bGameStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("Game has alreadd started"))
		return; 
	}
	
	if(!Controller->IsPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not a player controller")); 
		return; 
	}
	
	APlayerController* ConnectedPlayer = Cast<APlayerController>(Controller);
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

	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UAFGIMain>(GetGameInstance()); 
	}
	ConnectedPlayers.Add(ConnectedPlayer); 
	UE_LOG(LogTemp, Log, TEXT("Player with ID %i logged in sucessfully"), ConnectedPlayer->PlayerState->GetPlayerId())
	
	if(ConnectedPlayers.Num() >= GameInstance->GetConnectedPlayers()->Num())
	{
		OnAllPlayersConnected();
	}
	
}

void AThreeVsOneGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if(bGameStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("Game has alreadd started"))
		
		return; 
	}
	
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

	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UAFGIMain>(GetGameInstance()); 
	}
	ConnectedPlayers.Add(ConnectedPlayer); 
	UE_LOG(LogTemp, Log, TEXT("Player with ID %i logged in sucessfully"), ConnectedPlayer->PlayerState->GetPlayerId())
	

	
	if(ConnectedPlayers.Num() >= GameInstance->GetConnectedPlayers()->Num())
	{
		OnAllPlayersConnected();
	}
}
