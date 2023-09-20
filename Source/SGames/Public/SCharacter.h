// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// Included for struct FInputActionInstance (Enhanced Input)
#include "InputAction.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USAttributeComponent;
class USActionComponent;
class USInteractionComponent;

UCLASS()
class SGAMES_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USAttributeComponent> AttributeComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USActionComponent> ActionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USInteractionComponent> InteractionComp;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_LookMouse;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_Interact;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_Sprint;
		
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_PrimaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_SecondaryAttack;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_Dash;

	void Move(const FInputActionInstance& Instance);

	void LookMouse(const FInputActionValue& InputValue);

	void PrimaryInteract();

	void SprintStart();

	void SprintStop();

	void PrimaryAttack();

	void BlackHoleAttack();

	void Dash();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
