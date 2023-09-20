// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"


class USActionComponent;
/**
 * 
 */
UCLASS()
class SGAMES_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	/* Is Pawn Stunned based on GameplayTag data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FGameplayTag StunnedTag;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TObjectPtr<USActionComponent> ActionComp;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
