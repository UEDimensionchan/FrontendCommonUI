// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class MYWIDGET_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	static UFrontendGameUserSettings* Get();

	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficuty) { CurrentGameDifficulty = InNewDifficuty; }

private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
};
