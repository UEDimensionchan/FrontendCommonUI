// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendUISubsystem.h"
#include "Engine/AssetManager.h"
#include "../Widgets/Widget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "../Widgets/Widget_ActivatableBase.h"
#include "../Widgets/Widget_ConfirmScreen.h"
#include "../FrontendGameplayTags.h"
#include "../FrontendFunctionLibrary.h"

#include "../Debug/FrontendDebugHelper.h"

UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
	 	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}
	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}
	return false;
}

void UFrontendUISubsystem::RegisterCreatedPrimaryLayout(UWidget_PrimaryLayout* InCreatedPrimaryLayout)
{
	check(InCreatedPrimaryLayout);

	CreatedPrimaryLayout = InCreatedPrimaryLayout;
}

void UFrontendUISubsystem::PushSoftWidgetToStackAynsc(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AysncPushStateCallBack)
{
	check(!InSoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass,this, InWidgetStackTag, AysncPushStateCallBack]()
			{
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				check(LoadedWidgetClass && CreatedPrimaryLayout);

				UCommonActivatableWidgetContainerBase* FoundWigetStack = CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

				UWidget_ActivatableBase* CreateWidget = FoundWigetStack->AddWidget<UWidget_ActivatableBase>(
					LoadedWidgetClass,
					[&](UWidget_ActivatableBase& CreatedWidgetInstance)
					{
						AysncPushStateCallBack(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
						
					}
				);

				AysncPushStateCallBack(EAsyncPushWidgetState::AfterPush, CreateWidget);
			}
		)
	);
}

void UFrontendUISubsystem::PushConfirmScreenToMadalStackAynsc(EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg, TFunction<void(EConfirmScreenButtonType)> ButtonClickCallBack)
{
	UConfirmScreenInfoObject* CreatedInfoObject = nullptr;
	switch (InScreenType)
	{
	case EConfirmScreenType::Ok:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOKScreen(InScreenTitle, InScreenMsg);

		break;
	case EConfirmScreenType::YesNo:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMsg);

		break;
	case EConfirmScreenType::OkCancel:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkCancelScreen(InScreenTitle, InScreenMsg);

		break;
	case EConfirmScreenType::Unkown:
		break;
	default:
		break;
	}

	check(CreatedInfoObject);

	PushSoftWidgetToStackAynsc(
		FrontendGameplayTags::Frontend_WidgetStack_Modal,
		UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(FrontendGameplayTags::Frontend_Widget_ConfirmScreen),
		[CreatedInfoObject, ButtonClickCallBack](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_ConfirmScreen* CreatedConfirmScreen = CastChecked<UWidget_ConfirmScreen>(PushedWidget);
				CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject, ButtonClickCallBack);
			}
		}
	);
}
