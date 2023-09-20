// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <EnvironmentQuery/EnvQueryTypes.h>
#include "SGameModeBase.generated.h"


class UEnvQuery;
class ASAICharacter;
class UCurveFloat;
/**
 * 
 */
UCLASS()
class SGAMES_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameModeBase();

	virtual void StartPlay() override;

	void OnActorKilled(AActor* VictimActor, AActor* Killer);


protected:

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UEnvQuery> SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<ASAICharacter> MonsterClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UCurveFloat> SpawnBotCurve;

protected:

	UFUNCTION()
	void SpawnBotTimerElapsed();

	void OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);
};
