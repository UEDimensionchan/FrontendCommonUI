// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"


class UFrontendCommonButtonBase;
class UFrontendCommonRotator;
class UListDataObject_String;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class MYWIDGET_API UWidget_ListEntry_String : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;

	virtual void OnOnlyListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;

private:
	void OnPreviousOptionButtonClicked();
	void OnNextOptionButtonClicked();

	void OnRotatorValueChanged(int32 Value, bool bUserInitiated);
	
private:
	//Bound Widgets//
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_PreviousOption;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonRotator* CommonRotator_AvailableOptions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_NextOption;
	//Bound Widgets//

	UPROPERTY(Transient)
	UListDataObject_String* CachedOwningStringObject;
};
