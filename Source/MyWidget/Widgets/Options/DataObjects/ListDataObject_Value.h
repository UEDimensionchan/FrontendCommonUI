// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_Base.h"
#include "ListDataObject_Value.generated.h"

class FOptionsDataInteractionHelper;
/**
 * 
 */
UCLASS(Abstract)
class MYWIDGET_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGettter);
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSettter);

	void SetDefaultValueFromString(const FString& InDefaultValue) { DefaultStringValue = InDefaultValue; }
	
	virtual bool HasDefaultValue()const override { return DefaultStringValue.IsSet(); }
protected:
	FString GetDefaultValueAsString()const { return DefaultStringValue.GetValue(); }

	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;

private:
	TOptional<FString> DefaultStringValue;
};
