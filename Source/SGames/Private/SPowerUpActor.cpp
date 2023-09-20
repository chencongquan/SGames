// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpActor.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>

ASPowerUpActor::ASPowerUpActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

}


void ASPowerUpActor::Interact_Implementation(APawn* InstigatorPawn)
{
	Destroy();
}


FText ASPowerUpActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
    return FText::GetEmpty();
}
