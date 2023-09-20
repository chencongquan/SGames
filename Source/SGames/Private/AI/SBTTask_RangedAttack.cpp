// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include <AIController.h>
#include <GameFramework/Character.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "SAttributeComponent.h"






USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	HandSocketName = "FX_Weapon_R_Tip";
	MaxBulletSpread = 2.0f;
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
		if (ensure(AICharacter))
		{
			AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
			if (TargetActor == nullptr)
			{
				return EBTNodeResult::Failed;
			}

			if (!USAttributeComponent::IsActorAlive(TargetActor))
			{
				return EBTNodeResult::Failed;
			}

			FVector HandLocation = AICharacter->GetMesh()->GetSocketLocation(HandSocketName);
			FVector Direction = TargetActor->GetActorLocation() - HandLocation;
			FRotator HandRotation = Direction.Rotation();

			// Ignore negative pitch to not hit the floor in front itself
			HandRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
			HandRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			Params.Instigator = AICharacter;

			AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, HandLocation, HandRotation, Params);

			return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Failed;
}
