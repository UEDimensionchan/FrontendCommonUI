// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"


class AFrontendPlayerController;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class MYWIDGET_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure,BlueprintCallable)
	AFrontendPlayerController* GetOwningFrontendPlayerController();
	
private:
	TWeakObjectPtr<AFrontendPlayerController> CacheOwningFrontendPC;
};
