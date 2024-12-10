// Fill out your copyright notice in the Description page of Project Settings.


#include "AFGIMain.h"

#include "GameFramework/GameSession.h"
#include "Proj/MythbreakPlayerState.h"

void UAFGIMain::TravelServer() const
{
	const FString Command = "servertravel" + LevelFilePath;
	
	GetFirstLocalPlayerController()->ConsoleCommand(Command,true); 

}


const TMap<FUniqueNetIdRepl, FConnectedPlayer>* UAFGIMain::GetConnectedPlayers() const
{
	return &ConnectedPlayers; 
}

FGameModeSettings UAFGIMain::GetGameModeSettings() const
{
	return GameModeSettings; 
	
}

void UAFGIMain::CreateLanServer()
{
	auto Player = GetFirstLocalPlayerController();
	
	
}

void UAFGIMain::CreateOnlineServer()
{
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

void UAFGIMain::StartGameInstance()
{
	Super::StartGameInstance();
	ConnectedPlayers.Reserve(10);
}


void UAFGIMain::PrintAllUserIds()
{

	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerID: %i"), Iterator->Get()->PlayerState->PlayerId)
		UE_LOG(LogTemp, Warning, TEXT("UniqueID: %i"), Iterator->Get()->PlayerState->GetUniqueID())
		
	}

	
}

FConnectedPlayer UAFGIMain::GetPlayerInfo(APlayerController* Controller) const
{

	const auto MythState = GetMythBreakState(Controller);
	const auto PlayerInfo = ConnectedPlayers.Find(MythState->GetUniqueId());
	return *PlayerInfo; 
	
	
	
}

UAFGIMain::UAFGIMain(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConnectedPlayers.Reserve(10);
	UE_LOG(LogTemp, Warning, TEXT("I HAVE BEEN INITED")); 
	GameModeSettings = FGameModeSettings(1);
}


void UAFGIMain::StartGame()
{
	// Validates data
	for(FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		const auto Controller = Iterator->Get();
		
		const AMythbreakPlayerState* MythState = GetMythBreakState(Controller);

		if(!IsValid(MythState))
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid MythState")); 
			continue; 
		}

		if(!ConnectedPlayers.Contains(MythState->GetUniqueId()))
		{
			// Invalid player
			UE_LOG(LogTemp, Warning, TEXT("Invalid Player"))
			// kick player here
			continue;
		}
		
	}
	UE_LOG(LogTemp, Warning, TEXT("WE MOVING"))
	TravelServer(); 
}

void UAFGIMain::AddNewPlayer(const APlayerController* Controller, const bool IsBoss)
{
	if(this == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT(":(") )
		return;
	}

	AMythbreakPlayerState* MythState = GetMythBreakState(Controller);
	if(!IsValid(MythState))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid MtyhState")); 
		return; 
	}
	
	UE_LOG(LogTemp, Warning, TEXT("ID: %i "), MythState->PlayerId); 
	std::map<int, int> Test2;
	TMap<uint32, FConnectedPlayer> Test3; 

	const auto Key = MythState->GetUniqueId();
	
	if(ConnectedPlayers.Contains(Key))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has already been added"));
		return;
	}
	
	
	if(IsBoss)
	{
		ConnectedPlayers.Add(Key, FConnectedPlayer(DefaultBoss, TEXT(""), 0, true));

	}
	else
	{
		ConnectedPlayers.Add(Key, FConnectedPlayer(DefaultHero, TEXT(""), 0, false));

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
	if(ConnectedPlayers.Remove(MythState->GetUniqueId())>= 1 )
	{
		return true; 
	} 
	return false; 
}

void UAFGIMain::UpdateSelectedPlayer(const APlayerController* Controller, const FName NameOfCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerID: %i"), Controller->PlayerState->PlayerId)
	UE_LOG(LogTemp, Warning, TEXT("UniqueID: %i"), Controller->PlayerState->GetUniqueID())
	
	const AMythbreakPlayerState* MythState = GetMythBreakState(Controller);
	if(MythState == nullptr )
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid MythState")); 
		return; 
	}

	if(!ConnectedPlayers.Contains(MythState->GetUniqueId()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player does not exist"));
		return;
	}

	const auto Result = ConnectedPlayers.Find(MythState->GetUniqueId()); 
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

	if(!ConnectedPlayers.Contains(MythState->GetUniqueId()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player does not exist"));
		
		return;
	}

	const auto Result = ConnectedPlayers.Find(MythState->GetUniqueId()); 
	Result->SelectedSkin = NameOfSkin; 
	
}
