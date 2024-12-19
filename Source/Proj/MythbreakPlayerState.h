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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FPlayerStats PlayerStats;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddDamageDealt(float Damage);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddDamageTaken(float DamageTaken);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetDamageTaken() const;
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetDamageDealt() const;

	UPROPERTY(Replicated, BlueprintReadWrite)
	FString NameOfPlayer = "Player"; 

/*
	UPROPERTY(BlueprintReadWrite)
	int PlayerID;
	*/

};
