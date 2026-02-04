// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction_PushSoftWidget.h"
#include "../SubSystems/FrontendUISubsystem.h"
#include "../Widgets/Widget_ActivatableBase.h"



UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(
	const UObject* WorldContextObject, 
	APlayerController* OwningPlayerController, 
	TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, 
	UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InWidgetStackTag,
	bool bFocusOnNewLyPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("Push SoftWidget To Stack Was Passed a null Soft Widget class!"));

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushSoftWidget* Node = NewObject<UAsyncAction_PushSoftWidget>();
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewLyPushedWidget;


			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());

	FrontendUISubsystem->PushSoftWidgetToStackAynsc(CachedWidgetStackTag, CachedSoftWidgetClass,
		[this](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushWidget) {
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				PushWidget->SetOwningPlayer(CachedOwningPC.Get());
				OnWidgetCreatedBeforePush.Broadcast(PushWidget);

				break;
			case EAsyncPushWidgetState::AfterPush:

				AfterPush.Broadcast(PushWidget);
				if (bCachedFocusOnNewlyPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}

				SetReadyToDestroy();
				break;
			default:
				break;
			}
		}
	);
}
