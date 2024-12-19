#include "MythbreakPlayerState.h"

#include "Net/UnrealNetwork.h"

AMythbreakPlayerState::AMythbreakPlayerState()
{
}

void AMythbreakPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	/*
	PlayerState->SetPlayerId(this->GetPlayerId());
	PlayerState->SetUniqueId(this->GetUniqueId());
	*/
	const auto State = Cast<AMythbreakPlayerState>(PlayerState);
	if(State == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not a mythstate"))
		return; 
	}
	State->PlayerName = this->PlayerName; 
	UE_LOG(LogTemp, Warning, TEXT("COPY HAS BEEN CALLED"))
}

void AMythbreakPlayerState::AddDamageDealt(float Damage)
{
	PlayerStats.DamageDealt += Damage;
}

void AMythbreakPlayerState::AddDamageTaken(float DamageTaken)
{
	PlayerStats.DamageTaken += DamageTaken;
}

float AMythbreakPlayerState::GetDamageTaken() const
{
	return PlayerStats.DamageTaken;
}

float AMythbreakPlayerState::GetDamageDealt() const
{
	return PlayerStats.DamageDealt;
}



void AMythbreakPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMythbreakPlayerState, PlayerStats);
}


