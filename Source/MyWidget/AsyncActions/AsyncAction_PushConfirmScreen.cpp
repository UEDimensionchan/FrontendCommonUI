// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction_PushConfirmScreen.h"
#include "../SubSystems/FrontendUISubsystem.h"

UAsyncAction_PushConfirmScreen* UAsyncAction_PushConfirmScreen::PushConfirmScreen(
	const UObject* WorldContextObject, 
	EConfirmScreenType ScreenType, 
	FText InScreenTitle, 
	FText InScreenMessage)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		UAsyncAction_PushConfirmScreen* Node = NewObject<UAsyncAction_PushConfirmScreen>();
		Node->CachedOwningWorld = World;
		Node->CachedScreenType = ScreenType;
		Node->CachedScreenTitle = InScreenTitle;
		Node->CachedDScreenMsg = InScreenMessage;

		Node->RegisterWithGameInstance(World);

		return Node;
	}

	return nullptr;
}

void UAsyncAction_PushConfirmScreen::Activate()
{
	UFrontendUISubsystem::Get(CachedOwningWorld.Get())->PushConfirmScreenToMadalStackAynsc(
		CachedScreenType,
		CachedScreenTitle,
		CachedDScreenMsg,
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			OnButtonClicked.Broadcast(ClickedButtonType);

			SetReadyToDestroy();
		}
	);
}
