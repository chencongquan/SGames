// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>


void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	//APawn* TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (TargetPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", TargetPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetPawn);
	//}
}
