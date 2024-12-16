#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FPlayerStats.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY()
	float DamageDealt = 0 ;

	UPROPERTY()
	float DamageTaken = 0;

	FPlayerStats()
		: DamageDealt(0.0f), DamageTaken(0.0f){}
};
