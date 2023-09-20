// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "SGameModeBase.h"


USAttributeComponent::USAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HealthMax = 100;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	//如果不能受伤，并且是伤害值的话
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f )
	{
		return false;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelta = NewHealth - OldHealth;
	Health = NewHealth;
	
	//Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	return true;
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

float USAttributeComponent::GetRageMax() const
{
	return RageMax;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	return false;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);
}
