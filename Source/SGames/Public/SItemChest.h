// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class SGAMES_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();


private:


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bLidOpened;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LidMesh;

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch;

public:
	//接口重载
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	//暴露给蓝图的动画
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractAnimation();
};
