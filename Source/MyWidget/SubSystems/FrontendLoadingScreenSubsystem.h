// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendLoadingScreenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MYWIDGET_API UFrontendLoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, CurrentLoadingReason);

	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonUpdatedDelegate OnLoadingReasonUpdated;

	//~ Begin USubsytem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	//~ End USubsytem Interface

	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

private:
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);

	void OnMapPostLoaded(UWorld* LoadedWorld);

	bool IsPreLoadScreenActive() const;

	bool ShouldShowLoadingScreen();

	bool CheckTheNeedToShowLoadingScreen();

	void TryDisplayLoadingScreenIfNone();

	void TryUpdateLoadingScreen();

	void NotifyLoadingScreenVisibilityChanged(bool bIsVisible);

	void TryRemoveLoadingScreen();

private:
	bool bIsCurrentlyLoadingMap = false;

	float HoldLoadingScreenStartUpTime = -1.f;

	FString CurrentLoadingReason;

	TSharedPtr<SWidget> CachedCreatedLoadingScreenWidget;
};
