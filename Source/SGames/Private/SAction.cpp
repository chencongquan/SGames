// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "../SGames.h"


void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComp;
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	USActionComponent* Comp = GetOwningComponent();
	if (Comp && Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOGFMT(LogGame, Log, "Started: {ActionName}", GetName());
	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		bIsRunning = true;
		Comp->ActiveGameplayTags.AppendTags(GrantsTags);
		Comp->OnActionStarted.Broadcast(Comp, this);
		TimeStarted = GetWorld()->TimeSeconds;
	}

}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOGFMT(LogGame, Log, "Stopped: {name}", GetName());
	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		bIsRunning = false;
		Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
		Comp->OnActionStopped.Broadcast(Comp, this);
	}
}

UWorld* USAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	// 再ActionComponent.cpp里 USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	// 这个GetOuter() 获取的是ActionComp的Owner，也就是Pawn
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}
