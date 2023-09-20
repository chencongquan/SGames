// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "../SGames.h"
#include "SCharacter.h"
#include <EnvironmentQuery/EnvQueryManager.h>
#include <EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h>
#include <EngineUtils.h>
#include "SAttributeComponent.h"
#include "SPlayerState.h"



ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOGFMT(LogGame, Log, "OnActorKilled: Victim: {victim}, Killer: {killer}", GetNameSafe(VictimActor), GetNameSafe(Killer));

	// Handle Player death
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		//此时不能先调用UnPossess
		//AController* Con = Player->GetController();
		//if (Con == nullptr || Con->IsPendingKillPending())
		//{
		//	return;
		//}
		//玩家死亡
		//RestartPlayer(Player->GetController());
		ASPlayerState* PS = Player->GetPlayerState<ASPlayerState>();
		if (PS)
		{
			PS->RemoveCredits(10);
		}
	}
	else
	{
		// Give Credits for kill
		APawn* KillerPawn = Cast<APawn>(Killer);
		// Don't credit kills of self
		if (KillerPawn && KillerPawn != VictimActor)
		{
			// Only Players will have a 'PlayerState' instance, bots have nullptr
			ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
			if (PS)
			{
				PS->AddCredits(1);
			}
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	// Count alive bots before spawning
	int32 NrOfAliveBots = 0;
	// TActorRange simplifies the code compared to TActorIterator<T>
	for (ASAICharacter* Bot : TActorRange<ASAICharacter>(GetWorld()))
	{
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float SpawnMaxNum = 10.0f;
	if (SpawnBotCurve)
	{
		SpawnMaxNum = SpawnBotCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= SpawnMaxNum)
	{
		return;
	}
	//UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest25Pct, nullptr);
	//QueryInstance->GetOnQueryFinishedEvent().AddDyanmic(this, &ASGameModeBase::OnBotSpawnQueryCompleted);

	// Skip the Blueprint wrapper and use the direct C++ option which the Wrapper uses as well
	FEnvQueryRequest Request(SpawnBotQuery, this);
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ASGameModeBase::OnBotSpawnQueryCompleted);
}

void ASGameModeBase::OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOGFMT(LogGame, Warning, "Spawn bot EQS Query Failed!");
		return;
	}

	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);

	if (Locations.IsValidIndex(0))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* Bot = GetWorld()->SpawnActor<AActor>(MonsterClass, Locations[0], FRotator::ZeroRotator, SpawnParams);
		if (!Bot)
		{
			UE_LOGFMT(LogGame, Warning, "Spawn bot Failed!");
		}
		bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Locations[0], 30.0f, 32, FColor::Green, false, 5.0f);
		}
	}
}
