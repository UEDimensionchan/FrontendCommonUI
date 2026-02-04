// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_ListEntry_Scalar.h"

void UWidget_ListEntry_Scalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWidget_ListEntry_Scalar::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
}

void UWidget_ListEntry_Scalar::OnOnlyListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{

}
