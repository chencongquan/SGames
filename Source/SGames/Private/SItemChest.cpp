// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include <Components/StaticMeshComponent.h>

ASItemChest::ASItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;

	InteractAnimation();
}

void ASItemChest::InteractAnimation_Implementation()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}