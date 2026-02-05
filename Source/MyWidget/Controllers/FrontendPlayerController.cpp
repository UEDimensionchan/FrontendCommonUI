// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "../FontendSettings/FrontendGameUserSettings.h"

void AFrontendPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("Default"), FoundCameras);

	if (!FoundCameras.IsEmpty())
	{
		SetViewTarget(FoundCameras[0]);
	}

	UFrontendGameUserSettings* GameUserSettings = UFrontendGameUserSettings::Get();

	if (GameUserSettings->GetLastCPUBenchmarkResult() == -1.f || GameUserSettings->GetLastGPUBenchmarkResult() == -1.f)
	{
		GameUserSettings->RunHardwareBenchmark();
		GameUserSettings->ApplyHardwareBenchmarkResults();
	}
}
