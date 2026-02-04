// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "FrontendCommonRotator.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class MYWIDGET_API UFrontendCommonRotator : public UCommonRotator
{
	GENERATED_BODY()

public:
	void SetSelectedOptionByText(const FText& InTextOption);
	
};
