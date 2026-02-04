// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendGameUserSettings.h"


UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}
