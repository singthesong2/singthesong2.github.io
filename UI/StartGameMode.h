// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/AwaitHUD.h"
#include "UI/MainLoadingHUD.h"
#include "MainStartUI.h"
#include "StartController.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStartGameMode();
	UMainStartUI* GetMainStartHUDWidget() const
	{
		return MainStartHUDWidget;
	}
	UMainLoadingHUD* GetMainLoadingHUDWidget() const
	{
		return MainLoadingHUDWidget;
	}
	UAwaitHUD* GetAwaitHUDWidget() const
	{
		return AwaitHUDWidget;
	}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<UUserWidget> MainStartHUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<UUserWidget> MainLoadingHUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<UUserWidget> AwaitHUDWidgetClass;
	UPROPERTY()
		FTimerHandle timer;

	void LoadingControll();

	UMainStartUI* MainStartHUDWidget;
	UMainLoadingHUD* MainLoadingHUDWidget;
	UAwaitHUD* AwaitHUDWidget;
};
