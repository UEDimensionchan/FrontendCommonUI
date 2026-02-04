// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendGameUserSettings.h"


UFrontendGameUserSettings::UFrontendGameUserSettings()
	:OverallVolume(1.f)
{

}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UFrontendGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;

	//真正设置音量的地方
}
