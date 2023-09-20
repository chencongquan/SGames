// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"

#include "../SGames.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CanvasPanel.h>
#include <UMG/Public/Blueprint/WidgetLayoutLibrary.h>


void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();
		UE_LOGFMT(LogGame, Warning, "AttachedActor is no valid!");
		return;
	}

	FVector2D ScreenPosition;
	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition);
	if (bIsOnScreen)
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;
		if (RootCanvasPanel)
		{
			RootCanvasPanel->SetRenderTranslation(ScreenPosition);
		}
	}
	if(RootCanvasPanel)
	{
		RootCanvasPanel->SetVisibility(bIsOnScreen ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}
