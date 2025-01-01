// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "AFGIMain.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(!NewPlayer->IsPlayerController())
	{
		return;
	}
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UAFGIMain>(GetWorld()->GetGameInstance()); 
	}
	
	GameInstance->AddNewPlayer(NewPlayer, NewPlayer->IsLocalController());

	//
	SpawnPlayer(NewPlayer, GameInstance->GetPlayerInfo(NewPlayer)); 
	// Update all characters
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if(!Exiting->IsPlayerController())
	{
		return; 
	}
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UAFGIMain>(GetWorld()->GetGameInstance()); 
	}
	
	GameInstance->RemovePlayer(Cast<APlayerController>(Exiting));
	RemovePlayer(Cast<APlayerController>(Exiting)); 
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	const auto Gi = GetGameInstance();
	if(Gi == nullptr)
	{
		
		return; 
	}

	GameInstance = Cast<UAFGIMain>(Gi);

	if(GameInstance == nullptr)
	{
		
		return; 
	}

	
}

void ALobbyGameMode::ChangePlayerCharacter(APlayerController* Controller, FName NameOfCharacter)
{
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UAFGIMain>(GetWorld()->GetGameInstance()); 
	}
	
	GameInstance->UpdateSelectedPlayer(Controller, NameOfCharacter);
	
}

void ALobbyGameMode::SetPLayerAsSpectator(APlayerController* Controller)
{
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UAFGIMain>(GetWorld()->GetGameInstance()); 
	}
	
	GameInstance->UpdateSelectedPlayer(Controller, FName("Spectator"));
}

void ALobbyGameMode::GenericPlayerInitialization(AController* Controller)
{
	Super::GenericPlayerInitialization(Controller);

	if(bLoggedIn)
	{
		return; 
	}
	
	if(!Controller->IsPlayerController())
	{
		return; 
	}
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UAFGIMain>(GetWorld()->GetGameInstance()); 
	}
	
	
	const auto NewPlayer = Cast<APlayerController>(Controller); 
	if(!GameInstance->GetConnectedPlayers()->Contains(NewPlayer->PlayerState->GetUniqueId()))
	{
		return; 
	}
	SpawnPlayer(NewPlayer, GameInstance->GetPlayerInfo(NewPlayer)); 
}



