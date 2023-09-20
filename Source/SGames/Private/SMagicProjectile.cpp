// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "../SGames.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "SActionEffect.h"

ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.0f;
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOGFMT(LogGame, Log, "OtherActorName:{name}", GetNameSafe(OtherActor));
	UE_LOGFMT(LogGame, Warning, "OnActorOverlap:InstigatorActorName:{name},OtherActorName:{name}", GetNameSafe(GetInstigator()), GetNameSafe(OtherActor));
	//FColor Color = OtherActor == GetInstigator() ? FColor::Red : FColor::Blue;
	//LogOnScreen(this, FString::Printf(TEXT("OnActorOverlap:InstigatorActorName:{%s},OtherActorName:{%s}"), *GetNameSafe(GetInstigator()), *GetNameSafe(OtherActor)), Color);

	if (OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && BurningActionClass && USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			ActionComp->AddAction(GetInstigator(), BurningActionClass);
			Explode();
		}
	}
}
