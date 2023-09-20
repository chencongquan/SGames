// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"
#include "../SGames.h"


USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Since we use Camera info in Tick we want the most up to date camera position for tracing
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;

	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable()
{

	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	AActor* MyOwner = GetOwner();
	//获取Pawn的眼睛，也就是StartPos
	FVector EyeLocation;
	FRotator EyeRotation;
	//void APawn::GetActorEyesViewPoint(FVector & out_Location, FRotator & out_Rotation) const
	//{
	//	out_Location = GetPawnViewLocation();
	//	out_Rotation = GetViewRotation();
	//}
	// APawn 里重写了此函数，所以才能得到正确的眼睛的位置 和 Controller的Rotation
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Blue, false, 0.0f);
	}
	//碰撞查询通道
	FCollisionObjectQueryParams ObjectQueryParams;
	for (TEnumAsByte<ECollisionChannel> CollisionChannel : CollisionChannelArr)
	{
		ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	}

	// Ignore Player
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MyOwner);

	//碰撞图形
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	TArray<FHitResult> Hits;

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	// Clear ref before trying to fill
	FocusedActor = nullptr;
	if (bBlockingHit)
	{
		for (const FHitResult& Hit : Hits)
		{

			if (bDebugDraw)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 0.0f);
			}

			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}
	//if (GetWorld()->SweepMultiByChannel(Hits, TraceStart, TraceEnd, FQuat::Identity, COLLISION_PROJECTILE, Shape, Params))
	//{
	//}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}

void USInteractionComponent::PrimaryInteract()
{
	if (FocusedActor && FocusedActor->Implements<USGameplayInterface>())
	{
		APawn* MyPawn = Cast<APawn>(GetOwner());
		ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
	}
}

