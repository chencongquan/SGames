// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "../SGames.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "FX_Gun_Barrel";
	AttackAnimDelay = 0.2f;
	AttackBlockTime = 1.0f;
	SweepRadius = 20.0f;
	SweepDistanceFallback = 5000;
}


void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

		FTimerHandle TimerHandle_AttackAnimDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackAnimDelay, Delegate, AttackAnimDelay, false);


		FTimerHandle TimerHandle_AttackBlockTime;
		FTimerDelegate Delegate_Stop;
		Delegate_Stop.BindUObject(this, &USAction_ProjectileAttack::AttackDelay_StopAction, Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackBlockTime, Delegate_Stop, AttackBlockTime, false);
	}
}


void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(SweepRadius);
		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();

		const FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * SweepRadius);
		const FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);
		FVector AdjustedTraceEnd = TraceEnd;

		TArray<FHitResult> Hits;
		if (GetWorld()->SweepMultiByChannel(Hits, TraceStart, TraceEnd, FQuat::Identity, COLLISION_PROJECTILE, Shape, Params))
		{
			AdjustedTraceEnd = Hits[0].ImpactPoint;
		}

		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		FRotator ProjectileRot = (AdjustedTraceEnd - HandLocation).Rotation();
		FTransform SpawnTransform = FTransform(ProjectileRot, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}
}

void USAction_ProjectileAttack::AttackDelay_StopAction(ACharacter* InstigatorCharacter)
{
	StopAction(InstigatorCharacter);
}
