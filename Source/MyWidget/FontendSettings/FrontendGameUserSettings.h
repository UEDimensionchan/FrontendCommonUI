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
	UFrontendGameUserSettings();

	static UFrontendGameUserSettings* Get();

	//GameDifficultSettings
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficuty) { CurrentGameDifficulty = InNewDifficuty; }
	//GameDifficultSettings

	//AudioSettings
	UFUNCTION()
	float GetOverallVolume() const { return OverallVolume; }
	UFUNCTION()
	void SetOverallVolume(float InVolume);
	//AudioSettings

private:
	//GameDifficultSettings
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	//GameDifficultSettings

	//AudioSettings
	UPROPERTY(Config)
	float OverallVolume;
	//AudioSettings
};
