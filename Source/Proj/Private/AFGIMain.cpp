// Fill out your copyright notice in the Description page of Project Settings.


#include "AFGIMain.h"
#include "Proj/MythbreakPlayerState.h"

void UAFGIMain::TravelServer() const
{
	const FString Command = "servertravel " + LevelFilePath;

	GetWorld()->Exec(GetWorld(), *Command);
	
}

 TMap<int, FConnectedPlayer>* UAFGIMain::GetConnectedPlayers() const
{
	return ConnectedPlayers; 
}

FGameModeSettings UAFGIMain::GetGameModeSettings() const
{
	return GameModeSettings; 
	
}

AMythbreakPlayerState* UAFGIMain::GetMythBreakState(const APlayerController* Controller) const
{
	if(Controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid controller")); 
		return nullptr; 
	}
	
	const auto State = Controller->GetPlayerState<AMythbreakPlayerState>(); 
	if(State == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid state")); 
		return nullptr; 
	}
	
	return  State; 
}

void UAFGIMain::StartGame()
{

	// Validates data
	
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		const auto Controller = Iterator->Get();
		const AMythbreakPlayerState* MythState = GetMythBreakState(Controller);
		if(MythState == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid MythState")); 
			continue; 
		}

		if(!ConnectedPlayers->Contains(MythState->GetUniqueID()))
		{
			// Invalid player
			// kick player here
			continue;
		}
		
	}
	TravelServer(); 
}

void UAFGIMain::AddNewPlayer(const APlayerController* Controller, bool isBoss)
{

	const AMythbreakPlayerState* MythState = GetMythBreakState(Controller);
	if(MythState == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid MtyhState")); 
		return; 
	}

	if(ConnectedPlayers->Contains(MythState->GetUniqueID()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has already been added"));
		return;
	}
	
	if(isBoss)
	{
		ConnectedPlayers->Add(MythState->GetUniqueID(), FConnectedPlayer(DefaultBoss, TEXT(""), MythState->GetUniqueID(), true));
	}
	else
	{
		ConnectedPlayers->Add(MythState->GetUniqueID(), FConnectedPlayer(DefaultHero, TEXT(""), MythState->GetUniqueID(), false));
	}
	
}

bool UAFGIMain::RemovePlayer(const APlayerController* Controller)
{
	const AMythbreakPlayerState* MythState = GetMythBreakState(Controller);
	if(MythState == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid MythState")); 
		return false; 
	}
	if(ConnectedPlayers->Remove(MythState->GetUniqueID())>= 1 )
	{
		return true; 
	} 
	return false; 
}

void UAFGIMain::UpdateSelectedPlayer(const APlayerController* Controller, const FName NameOfCharacter)
{

	const AMythbreakPlayerState* MythState = GetMythBreakState(Controller);
	if(MythState == nullptr )
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid MythState")); 
		return; 
	}

	if(!ConnectedPlayers->Contains(MythState->GetUniqueID()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player does not exist"));
		return;
	}

	const auto Result = ConnectedPlayers->Find(MythState->GetUniqueID()); 
	Result->SelectedCharacter = NameOfCharacter; 

	
}

void UAFGIMain::UpdateSelectedSkin(const APlayerController* Controller, const FName NameOfSkin)
{
	
	const AMythbreakPlayerState* MythState = GetMythBreakState(Controller);
	if(MythState == nullptr )
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid MythState")); 
		return; 
	}

	if(!ConnectedPlayers->Contains(MythState->GetUniqueID()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player does not exist"));
		return;
	}

	const auto Result = ConnectedPlayers->Find(MythState->GetUniqueID()); 
	Result->SelectedSkin = NameOfSkin; 
	
}
