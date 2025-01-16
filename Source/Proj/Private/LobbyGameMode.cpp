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

	if(GetGameAfgiInstance()->GetAmountOfNoneSpectators() >= NumbersOfNoneSpectators)
	{
		GetGameAfgiInstance()->AddSpectator(NewPlayer);
		SpawnPlayerAsSpectator(NewPlayer);
		return; 
	}
	
	GetGameAfgiInstance()->AddNewPlayer(NewPlayer, NewPlayer->IsLocalController());
	SpawnPlayer(NewPlayer, GetGameAfgiInstance()->GetPlayerInfo(NewPlayer)); 
	// Update all characters
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if(!Exiting->IsPlayerController())
	{
		return; 
	}
	GetGameAfgiInstance()->RemovePlayer(Cast<APlayerController>(Exiting));
	RemovePlayer(Cast<APlayerController>(Exiting)); 
}

UAFGIMain* ALobbyGameMode::GetGameAfgiInstance()
{
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UAFGIMain>(GetWorld()->GetGameInstance()); 
	}
	return GameInstance; 
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyGameMode::ChangePlayerCharacter(APlayerController* Controller, FName NameOfCharacter)
{
	GetGameAfgiInstance()->UpdateSelectedPlayer(Controller, NameOfCharacter);
}

void ALobbyGameMode::SetPLayerAsSpectator(APlayerController* Controller)
{
	GetGameAfgiInstance()->UpdateSelectedPlayer(Controller, FName("Spectator"));
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
	
	const auto NewPlayer = Cast<APlayerController>(Controller); 
	if(!GetGameAfgiInstance()->GetConnectedPlayers()->Contains(NewPlayer->PlayerState->GetUniqueId()))
	{
		return; 
	}
	SpawnPlayer(NewPlayer, GetGameAfgiInstance()->GetPlayerInfo(NewPlayer)); 
}

void ALobbyGameMode::SetReadyStateForPlayer(const APlayerController* PlayerController, const bool IsReady)
{
	ReadyList.Add(PlayerController, IsReady);
	OnReadyStateChanged(PlayerController, IsReady); 
}

bool ALobbyGameMode::GetReadyStateForPlayer(const APlayerController* PlayerController)
{
	if(!ReadyList.Contains(PlayerController))
	{
		ReadyList.Add(PlayerController, false);
	}
	
	return ReadyList[PlayerController];
}

bool ALobbyGameMode::ArePlayersReady() const
{

	
	
	for (const auto PresenceKey : ReadyList)
	{
		if(PresenceKey.Key == nullptr)
		{
			continue;
		}

		const auto PlayerInfo = GameInstance->GetPlayerInfo(PresenceKey.Key);

		if(!PlayerInfo.SelectedCharacter.IsValid())
		{
			continue; 
		}

		if(PlayerInfo.SelectedCharacter == FName("Spectator"))
		{
			continue;
		}
		
		if (!PresenceKey.Value)
		{
			return false;
		}
	}
	
	return true;
}

void ALobbyGameMode::RemoveReadyState(const APlayerController* Controller)
{
	if(Controller == nullptr)
	{
		return;
	}
	
	if(!ReadyList.Contains(Controller))
	{
		return;
	}
	
	ReadyList.Remove(Controller);
}

bool ALobbyGameMode::MaxPlayersReached() const
{
	// Makes sure it stays const
	const auto Gm =  Cast<UAFGIMain>(GetWorld()->GetGameInstance()); 
	return Gm->GetAmountOfNoneSpectators() >= NumbersOfNoneSpectators; 
}



