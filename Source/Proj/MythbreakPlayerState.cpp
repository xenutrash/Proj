#include "MythbreakPlayerState.h"

#include "Net/UnrealNetwork.h"

AMythbreakPlayerState::AMythbreakPlayerState()
{
}

void AMythbreakPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	PlayerState->SetPlayerId(this->GetPlayerId());
	PlayerState->SetUniqueId(this->GetUniqueId());
	
	UE_LOG(LogTemp, Warning, TEXT("COPY HAS BEEN CALLED"))
	
	auto MythState = Cast<AMythbreakPlayerState>(PlayerState);
	if(MythState == nullptr)
	{
		return; 
	}

	MythState->SetTestID(this->TestId);
	
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

void AMythbreakPlayerState::SetTestID(int Value)
{
	TestId = Value; 
}

void AMythbreakPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMythbreakPlayerState, PlayerStats);
}


