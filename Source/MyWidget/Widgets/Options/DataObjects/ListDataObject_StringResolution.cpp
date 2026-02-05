// Fill out your copyright notice in the Description page of Project Settings.


#include "ListDataObject_StringResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../OptionsDataInteractionHelper.h"
#include "../../../FontendSettings/FrontendGameUserSettings.h"

#include "../../../Debug/FrontendDebugHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvaialbleResolutions;

	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvaialbleResolutions);

	AvaialbleResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B)->bool
		{
			return A.SizeSquared() < B.SizeSquared();
		}
	);

	for (const FIntPoint& Resolution : AvaialbleResolutions)
	{
		{
			AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
		}

		MaximumAllowedResolution = ResToValueString(AvaialbleResolutions.Last());

		SetDefaultValueFromString(MaximumAllowedResolution);
	}

}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResToDisplayText(UFrontendGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution) const
{
	return FString::Printf(TEXT("(X=%i,Y=%i)"), InResolution.X, InResolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	const FString DisplayString = FString::Printf(TEXT("%i x %i"), InResolution.X, InResolution.Y);

	return FText::FromString(DisplayString);
}
