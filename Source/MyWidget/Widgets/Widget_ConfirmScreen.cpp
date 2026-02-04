// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_ConfirmScreen.h"
#include "Components/DynamicEntryBox.h"
#include "CommonTextBlock.h"
#include "Components/FrontendCommonButtonBase.h"
#include "ICommonInputModule.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
    UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
    InfoObject->ScreenTitle = InScreenTitle;
    InfoObject->ScreenMessage = InScreenMsg;

    FConfirmScreenButtonInfo OKButtonInfo;
    OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
    OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));

    InfoObject->AvailableScreenButtons.Add(OKButtonInfo);

    return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
    UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
    InfoObject->ScreenTitle = InScreenTitle;
    InfoObject->ScreenMessage = InScreenMsg;

    FConfirmScreenButtonInfo YesButtonInfo;
    YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
    YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Yes"));

    FConfirmScreenButtonInfo NoButtonInfo;
    NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
    NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("No"));

    InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
    InfoObject->AvailableScreenButtons.Add(NoButtonInfo);

    return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
    UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
    InfoObject->ScreenTitle = InScreenTitle;
    InfoObject->ScreenMessage = InScreenMsg;

    FConfirmScreenButtonInfo OkButtonInfo;
    OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
    OkButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));

    FConfirmScreenButtonInfo CancelButtonInfo;
    CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
    CancelButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Cancel"));

    InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
    InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);

    return InfoObject;
}

void UWidget_ConfirmScreen::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallBack)
{
    check(InScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);

    CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
    CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);

    if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
    {
        DynamicEntryBox_Buttons->Reset<UFrontendCommonButtonBase>(
            [](UFrontendCommonButtonBase& ExistingButton)
            {
                ExistingButton.OnClicked().Clear();
            }
        );
    }

    check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());

    for (const FConfirmScreenButtonInfo& AvailableButtonInfo : InScreenInfoObject->AvailableScreenButtons)
    {
        FDataTableRowHandle InputActionRowHandle;

        switch (AvailableButtonInfo.ConfirmScreenButtonType)
        {
        case EConfirmScreenButtonType::Cancelled:
            InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
            break;

        case EConfirmScreenButtonType::Closed:
            InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
            break;

        default:
            break;
        }
        UFrontendCommonButtonBase* AddedButton = DynamicEntryBox_Buttons->CreateEntry<UFrontendCommonButtonBase>();
        AddedButton->SetButtonText(AvailableButtonInfo.ButtonTextToDisplay);
        AddedButton->SetTriggeringInputAction(InputActionRowHandle);

        AddedButton->OnClicked().AddLambda(
            [ClickedButtonCallBack, AvailableButtonInfo,this]()
            {
                ClickedButtonCallBack(AvailableButtonInfo.ConfirmScreenButtonType);
                DeactivateWidget();
            }
        );
    }

    if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
    {
        DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
    }
}
