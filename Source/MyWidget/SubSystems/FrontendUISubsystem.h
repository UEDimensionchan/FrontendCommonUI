// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../FrontendTypes/FrontendEnumType.h"
#include "FrontendUISubsystem.generated.h"

class UWidget_PrimaryLayout;
struct FGameplayTag;
class UWidget_ActivatableBase;
class UFrontendCommonButtonBase;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdateDelegate, UFrontendCommonButtonBase*, BroadcastingButton, FText, DescriptionText);
/**
 * 
 */
UCLASS()
class MYWIDGET_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);

	//~Begine USubSystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~End USubSystem Interface

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayout(UWidget_PrimaryLayout* InCreatedPrimaryLayout);

	void PushSoftWidgetToStackAynsc(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,TFunction<void(EAsyncPushWidgetState,UWidget_ActivatableBase*)> AysncPushStateCallBack);

	void PushConfirmScreenToMadalStackAynsc(EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg, TFunction<void(EConfirmScreenButtonType)> ButtonClickCallBack);

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdateDelegate OnButtonDescriptionTextUpdate;


private:
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;
};
