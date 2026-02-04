// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_OptionsScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "OptionsDataRegistry.h"
#include "../Components/FrontendTabListWidgetBase.h"
#include "DataObjects/ListDataObject_Collection.h"
#include "../Components/FrontendCommonListView.h"
#include "../../FontendSettings/FrontendGameUserSettings.h"
#include "ListEntries/Widget_ListEntry_Base.h"
#include "Widget_OptionsDetailsView.h"
#include "../../SubSystems/FrontendUISubsystem.h"
#include "../Components/FrontendCommonButtonBase.h"

#include "../../Debug/FrontendDebugHelper.h"

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
		);
	}
	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);

	TabListWidget_OptionTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);

	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnLstViewItemHovered);
	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection)
		{
			continue;
		}
		const FName TabID = TabCollection->GetDataID();
		if (TabListWidget_OptionTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{
			continue;
		}

		TabListWidget_OptionTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
}

void UWidget_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

}

UWidget* UWidget_OptionsScreen::NativeGetDesiredFocusTarget() const
{
	if (UObject* SelectObject = CommonListView_OptionsList->GetSelectedItem())
	{
		if (UUserWidget* SelectedEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(SelectObject))
		{
			return SelectedEntryWidget;
		}
	}

	return Super::NativeGetDesiredFocusTarget();
}

void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	if (ResettableDataArray.IsEmpty())
	{
		return;
	}
	UCommonButtonBase* SelectTabButton = TabListWidget_OptionTabs->GetTabButtonBaseByID(TabListWidget_OptionTabs->GetActiveTab());


	const FString SelectedButtonText = CastChecked<UFrontendCommonButtonBase>(SelectTabButton)->GetButtonDisplayText().ToString();
	

	UFrontendUISubsystem::Get(this)->PushConfirmScreenToMadalStackAynsc(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset")),
		FText::FromString(TEXT("Are you sure you want to reset all the setting under the ") + SelectedButtonText + TEXT(" tab.")),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;
			}

			bIsResettingData = true;
			bool bHasDataFailedToReset = false;

			for (UListDataObject_Base* DataReset : ResettableDataArray)
			{
				if (!DataReset)
				{
					continue;
				}

				if (DataReset->TryResetBackToDefaultValue())
				{
					Debug::Print(DataReset->GetDataDisplayName().ToString() + TEXT(" was reset"));
				}
				else
				{
					bHasDataFailedToReset = true;
					Debug::Print(DataReset->GetDataDisplayName().ToString() + TEXT(" failed to reset"));
				}
			}
			if (!bHasDataFailedToReset)
			{
				ResettableDataArray.Empty();
				RemoveActionBinding(ResetActionHandle);
			}

			bIsResettingData = false;
		}
	);
}

void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();

	UFrontendGameUserSettings::Get()->ApplySettings(true);
}


UOptionsDataRegistry* UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>();
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}
	checkf(CreatedOwningDataRegistry, TEXT("Data registry for options screen is not valid"));

	return CreatedOwningDataRegistry;
}


void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabId)
{
	DetailsView_ListEntryInfo->ClearDetailsViewInfo();

	TArray<UListDataObject_Base*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemBySelectedTabID(TabId);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}

	ResettableDataArray.Empty();

	for (UListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}
		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewDataModified);
		}
		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UWidget_OptionsScreen::OnLstViewItemHovered(UObject* InHoveredItem, bool bwasHovered)
{
	if (!InHoveredItem)
	{
		return;
	}
	UWidget_ListEntry_Base* HoveredEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);
	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bwasHovered);

	if (bwasHovered)
	{ 
		DetailsView_ListEntryInfo->UpadateDetailsViewInfo(
			CastChecked<UListDataObject_Base>(InHoveredItem),
			TryGetEntryWidgetClassName(InHoveredItem)
		);
	}
	else
	{
		if (UListDataObject_Base* SelectItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
		{
			DetailsView_ListEntryInfo->UpadateDetailsViewInfo(
				SelectItem,
				TryGetEntryWidgetClassName(SelectItem)
			);
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
	{
		return;
	}

	DetailsView_ListEntryInfo->UpadateDetailsViewInfo(
		CastChecked<UListDataObject_Base>(InSelectedItem),
		TryGetEntryWidgetClassName(InSelectedItem)
	);
	
}

FString UWidget_OptionsScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	if(UUserWidget* FoundEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem))
	{
		return FoundEntryWidget->GetClass()->GetName();
	}

	return TEXT("Entry Widget Not Valid");
}

void UWidget_OptionsScreen::OnListViewDataModified(UListDataObject_Base* ModifyData, EOptionsListDataModifyReason ModifyReason)
{
	if (bIsResettingData || !ModifyData)
	{
		return;
	}

	if (!ModifyData)
	{
		return;
	}

	if (ModifyData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifyData);
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(ModifyData))
		{
			ResettableDataArray.Remove(ModifyData);
		}
	}
	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}
