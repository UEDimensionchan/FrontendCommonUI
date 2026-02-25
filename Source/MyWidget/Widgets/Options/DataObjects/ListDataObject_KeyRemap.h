// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_Base.h"

#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include "ListDataObject_KeyRemap.generated.h"


class UEnhancedInputUserSettings;
class UEnhancedPlayerMappableKeyProfile;
/**
 * 
 */
UCLASS()
class MYWIDGET_API UListDataObject_KeyRemap : public UListDataObject_Base
{
	GENERATED_BODY()


public:
	FORCEINLINE ECommonInputType GetDesiredInputKeyType() const { return CachedDesiredInputKeyType; }
	

public:
	void InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings, UEnhancedPlayerMappableKeyProfile* InKeyProfile, ECommonInputType InDesiredInputKeyType, const FPlayerKeyMapping& InOwningPlayerKeyMapping);

	FSlateBrush GetIconFromCurrentKey() const;

	void BindNewInputKey(const FKey& InNewKey);

	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

private:
	FPlayerKeyMapping* GetOwningKeyMapping() const;

private:
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedOwningInputUserSettings;

	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;

	ECommonInputType CachedDesiredInputKeyType;

	FName CachedOwningMappingName;

	EPlayerMappableKeySlot CachedOwningMappableKeySlot;
};
