#include "MythbreakPlayerState.h"

#include "Net/UnrealNetwork.h"


AMythbreakPlayerState::AMythbreakPlayerState()
{



}

void AMythbreakPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMythbreakPlayerState, NameOfPlayer); 
}


void AMythbreakPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	const auto State = Cast<AMythbreakPlayerState>(PlayerState);
	if(State == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not a mythstate"))
		return; 
	}
	if(State->NameOfPlayer == this->NameOfPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("NAMES ARE THE SAME"))
	}
	State->NameOfPlayer = this->NameOfPlayer; 
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






