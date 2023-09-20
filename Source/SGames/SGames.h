// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// Added here to more easily include whenever we also use LogGame (structuredlog is new in 5.2)
#include "Logging/StructuredLog.h"

#include <HAL/IConsoleManager.h>


// Define category "LogGame"
SGAMES_API DECLARE_LOG_CATEGORY_EXTERN(LogGame, Log, All);

#define COLLISION_PROJECTILE ECC_GameTraceChannel1

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.DebugDraw"), false, TEXT("Enable Debug Lines."), ECVF_Cheat);

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}