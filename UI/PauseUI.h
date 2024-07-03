// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "PauseUI.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API UPauseUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* GameOver;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* ReStartButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* NewStartButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* ExitButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* ReStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* NewStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* Exit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* GameOverImage;
	UPROPERTY()
		FTimerHandle timer;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UBackgroundBlur* Background;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
		class UWidgetAnimation* BackBlur;

	bool MouseOnOff = false;
	void DeadPauseOn();
	void Pause();
	void StopBlur();
	void DeleteSave();
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION(BlueprintCallable)
		void ReStartButtonCallback();
	UFUNCTION(BlueprintCallable)
		void NewStartButtonCallback();
	UFUNCTION(BlueprintCallable)
		void ExitButtonCallback();
};
