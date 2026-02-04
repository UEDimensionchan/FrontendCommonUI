// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsDetailsView.generated.h"


class UCommonTextBlock;
class UCommonLazyImage;
class UCommonRichTextBlock;
class UListDataObject_Base;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class MYWIDGET_API UWidget_OptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpadateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName = FString());
	void ClearDetailsViewInfo();

protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CommonLazyImage_DescriptionImage;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_Description;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DynamicDetails;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DisabledReason;
};
