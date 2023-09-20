// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class UCanvasPanel;

UCLASS()
class SGAMES_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> RootCanvasPanel;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn=true))
	TObjectPtr<AActor> AttachedActor;
};
