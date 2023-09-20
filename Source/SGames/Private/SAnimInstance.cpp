// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<USActionComponent>(OwningActor->GetComponentByClass(USActionComponent::StaticClass()));
		AttributeComponent = Cast<USAttributeComponent>(OwningActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
