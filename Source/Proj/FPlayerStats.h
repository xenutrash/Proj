#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FPlayerStats.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY()
	float DamageDealt;

	UPROPERTY()
	float DamageTaken;

	FPlayerStats()
		: DamageDealt(0.0f), DamageTaken(0.0f){}
};
