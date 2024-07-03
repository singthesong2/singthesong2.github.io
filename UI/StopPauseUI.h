// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/Border.h"
#include "Components/BackgroundBlur.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "StopPauseUI.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API UStopPauseUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Pause;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* ResumeButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Resume;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* NewGameButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* NewGame;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* LoadGameButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* LoadGame;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* ExitGameButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* ExitGame;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBackgroundBlur* Background;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* TitleLogo;

	void OptionOn();
	void OptionOff();
	bool MouseOn = false;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION(BlueprintCallable)
		void ResumeButtonCallback();
	UFUNCTION(BlueprintCallable)
		void NewGameButtonCallback();
	UFUNCTION(BlueprintCallable)
		void LoadGameButtonCallback();
	UFUNCTION(BlueprintCallable)
		void ExitGameButtonCallback();
	void DeleteSave();
};
