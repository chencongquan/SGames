// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"
#include "SWorldUserWidget.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include <Perception/PawnSensingComponent.h>

// Sets default values
ASAICharacter::ASAICharacter()
{
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	// Ensures we receive a controlled when spawned in the level by our gamemode
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Enabled on mesh to react to incoming projectiles
	GetMesh()->SetGenerateOverlapEvents(true);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	TargetActorKey = "TargetActor";

	GetCharacterMovement()->bUseControllerDesiredRotation = true; // 向Controller方向转身
	GetCharacterMovement()->bOrientRotationToMovement = false; // 向运动方向转身
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0); //转身速度
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (ActiveHealthBar == nullptr)
	{
		ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
		}
	}

	if (InstigatorActor != this)
	{
		SetTargetActor(InstigatorActor);
	}

	// Died
	if (NewHealth <= 0.0f)
	{
		// stop BT
		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC && ensure(AIC->GetBrainComponent()))
		{
			AIC->GetBrainComponent()->StopLogic("Killed");
		}

		// ragdoll
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();

		// set lifespan
		SetLifeSpan(10.0f);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->RemoveFromParent();
		}
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	if (GetTargetActor() == NewTarget)
	{
		//已经设置了
		return;
	}

	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);

		//有可能有些AI没有controller
		USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (NewWidget)
		{
			NewWidget->AttachedActor = this;
			// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
			// May end up behind the minion health bar otherwise.
			NewWidget->AddToViewport(10);
		}
	}

	
}

AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}
