// Fill out your copyright notice in the Description page of Project Settings.


#include "ListDataObject_Value.h"

void UListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGettter)
{
	DataDynamicGetter = InDynamicGettter;
}

void UListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSettter)
{
	DataDynamicSetter = InDynamicSettter;
}
