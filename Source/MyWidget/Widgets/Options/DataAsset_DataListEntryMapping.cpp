// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset_DataListEntryMapping.h"
#include "DataObjects/ListDataObject_Base.h"
#include "ListEntries/Widget_ListEntry_Base.h"

TSubclassOf<UWidget_ListEntry_Base> UDataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(UListDataObject_Base* InDataObject) const
{
	check(InDataObject);

	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass -> GetSuperClass())
	{
		if (TSubclassOf<UListDataObject_Base> ConvertDataObjectClass = TSubclassOf<UListDataObject_Base>(DataObjectClass))
		{
			if (DataObjectListEntryMap.Contains(ConvertDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertDataObjectClass);
			}
		}
	}
	return TSubclassOf<UWidget_ListEntry_Base>();
}
