// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendFunctionLibrary.h"
#include "FontendSettings/FrontendDeveloperSettings.h"

TSoftClassPtr<UWidget_ActivatableBase> UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetTag)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

	checkf(FrontendDeveloperSettings->FrontendWidgetMap.Contains(InWidgetTag), TEXT("Could Not Find Corresponding widget under the tag %s"), *InWidgetTag.ToString());

	return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}


TSoftObjectPtr<UTexture2D> UFrontendFunctionLibrary::GetOptionsSoftImageTag(UPARAM(meta = (Categories = "Frontend.Image")) FGameplayTag InImageTag)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

	checkf(FrontendDeveloperSettings->OptionsScreenSoftImagMap.Contains(InImageTag), TEXT("Could not find an image accociated with tag %s"), *InImageTag.ToString());
	
	return FrontendDeveloperSettings->OptionsScreenSoftImagMap.FindRef(InImageTag);
}