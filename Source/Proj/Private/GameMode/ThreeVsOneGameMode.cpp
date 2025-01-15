// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ThreeVsOneGameMode.h"

#include "AFGIMain.h"
#include "RushCharacter.h"
#include "Proj/MythbreakPlayerState.h"


UAFGIMain* AThreeVsOneGameMode::GetGameInstance()
{
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UAFGIMain>(GetWorld()->GetGameInstance()); 
	}
	return GameInstance; 
}

void AThreeVsOneGameMode::OnAllPlayersConnected()
{
	
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
		
		const FConnectedPlayer* PlayerInfo = GetGameInstance()->GetConnectedPlayers()->Find(MythState->GetUniqueId());

		if(PlayerInfo == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("The player does not have a valid playerID")); 
			continue;
		}
		
		PlayerInfo->PlayerIndex = Index;
		
		UE_LOG(LogTemp, Log, TEXT("Spawning player with ID %i as %s "), Controller->GetUniqueID(), *PlayerInfo->SelectedCharacter.ToString() );
		if(PlayerInfo->SelectedCharacter.IsEqual(FName("Spectator")))
		{
			OnSpawnSpectator(Controller, *PlayerInfo);
			UE_LOG(LogTemp, Log, TEXT("Spawning player as spectator")); 
		}else
		{
			OnSpawnPlayer(Controller, *PlayerInfo, GetGameInstance()->GetGameModeSettings());
		}
		
		
		Index++; 
	}
	UE_LOG(LogTemp, Warning, TEXT("All players spawned"));
	OnPlayersSpawned(GameInstance->GetGameModeSettings());
}

void AThreeVsOneGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AThreeVsOneGameMode::GenericPlayerInitialization(AController* Controller)
{
	Super::GenericPlayerInitialization(Controller);

	if(bGameStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("Game has already started"))
		if(Controller->IsPlayerController())
		{
			const FConnectedPlayer FakeInfo = FConnectedPlayer();
			const auto PlayerController = Cast<APlayerController>(Controller); 
			OnSpawnSpectator(PlayerController, FakeInfo); 
		}

		return; 
	}
	if(Controller == nullptr)
	{
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


	ConnectedPlayers.Add(ConnectedPlayer); 
	UE_LOG(LogTemp, Log, TEXT("Player with ID %i logged in sucessfully"), ConnectedPlayer->PlayerState->GetPlayerId())
	
	if(ConnectedPlayers.Num() >= GetGameInstance()->GetConnectedPlayers()->Num())
	{
		OnAllPlayersConnected();
	}
	
}

void AThreeVsOneGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if(Exiting == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Logout; Controller is null"))
		return; 
	}
	
	if(Exiting->IsPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Logout: Not a player controller"))
		return;
	}
	const auto Controller = Cast<APlayerController>(Exiting);
	if(Controller == nullptr)
	{
		return; 
	}
	
	GetGameInstance()->RemovePlayer(Controller);
	
	if(!bGameStarted)
	{
		return; 
	}

	const auto ControlledPawn = Exiting->GetPawn();
	if(ControlledPawn == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Logout: Controller does not have a pawn"))
		return; 
	}


	const auto RushCharacter = Cast<ARushCharacter>(ControlledPawn);
	
	if(RushCharacter == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Logout: Controlled pawn is not a rush character"))
		return; 
	}

	
	
}

void AThreeVsOneGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if(bGameStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("PostLogin: Game has alreadd started"))
		
		return; 
	}

	if(NewPlayer == nullptr)
	{
		return;
	}
	
	if(!NewPlayer->IsPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("PostLogin: Not a player controller")); 
		return; 
	}
	
	APlayerController* ConnectedPlayer = Cast<APlayerController>(NewPlayer);
	if(ConnectedPlayer == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PostLogin: Failed to cast to a player controller")); 
		return; 
	}
	if(ConnectedPlayers.Contains(ConnectedPlayer))
	{
		// this should never be the case
		UE_LOG(LogTemp, Warning, TEXT("PostLogin: Player has already connected")); 
		return; 
	}


	ConnectedPlayers.Add(ConnectedPlayer); 
	UE_LOG(LogTemp, Log, TEXT("PostLogin: Player with ID %i logged in successfully"), ConnectedPlayer->PlayerState->GetPlayerId())
	

	
	if(ConnectedPlayers.Num() >= GetGameInstance()->GetConnectedPlayers()->Num())
	{
		OnAllPlayersConnected();
	}
}
