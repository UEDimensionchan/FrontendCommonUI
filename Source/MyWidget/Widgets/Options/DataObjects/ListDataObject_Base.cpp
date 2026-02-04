// Fill out your copyright notice in the Description page of Project Settings.


#include "ListDataObject_Base.h"
#include "../../../FontendSettings/FrontendGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}


void UListDataObject_Base::OnDataObjectInitialized()
{

}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeImmediatly)
	{
		UFrontendGameUserSettings::Get()->ApplySettings(true);
	}
}
