#pragma once

#include "FPlayerStats.h"
#include "GameFramework/PlayerState.h"
#include "MythbreakPlayerState.generated.h"

UCLASS()
class PROJ_API AMythbreakPlayerState: public APlayerState
{
	GENERATED_BODY()
public:
	AMythbreakPlayerState();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FPlayerStats PlayerStats;

	void AddDamageDealt(float Damage);

	void AddDamageTaken(float DamageTaken);
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
