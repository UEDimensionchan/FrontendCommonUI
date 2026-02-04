// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYWIDGET_API AFrontendPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	//~Begine APlayerController Interface
	virtual void OnPossess(APawn* aPawn) override;
	//~End 
};
