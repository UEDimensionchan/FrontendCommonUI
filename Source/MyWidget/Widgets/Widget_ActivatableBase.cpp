// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_ActivatableBase.h"
#include  "../Controllers/FrontendPlayerController.h"

AFrontendPlayerController* UWidget_ActivatableBase::GetOwningFrontendPlayerController()
{
	if (!CacheOwningFrontendPC.IsValid())
	{
		CacheOwningFrontendPC = GetOwningPlayer<AFrontendPlayerController>();
	}
	return CacheOwningFrontendPC.IsValid() ? CacheOwningFrontendPC.Get() : nullptr;
}
