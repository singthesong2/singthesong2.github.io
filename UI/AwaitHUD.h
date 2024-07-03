// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "AwaitHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API UAwaitHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UAwaitHUD(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* AwaitBack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* AwaitText;
	UPROPERTY()
		class UMediaPlayer* MediaPlayer;
	UPROPERTY()
		class UMediaSource* Video;
	void HUDOn();
	void HUDOff();

protected:
	virtual void NativeConstruct() override;
};
