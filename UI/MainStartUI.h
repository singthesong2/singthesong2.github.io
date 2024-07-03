// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "MainStartUI.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API UMainStartUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainStartUI(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* MainBackGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* MainTitleImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* MainName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* MainStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* MainExit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* MainStartButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* MainExitButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* GameSelectButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* GameSelect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Stage1Button;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Stage2Button;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Stage3Button;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* BackButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* Stage1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* Stage2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* Stage3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* StageImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* StageText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* MissionText;
	//UPROPERTY()
	//	class UTexture2D* MapText1;
	//UPROPERTY()
	//	class UTexture2D* MapText2;
	//UPROPERTY()
	//	class UTexture2D* MapText3;

	UPROPERTY()
		class UMaterial* MapMat1;
	UPROPERTY()
		class UMaterialInstance* MapMat2;
	UPROPERTY()
		class UMaterialInstance* MapMat3;
	UPROPERTY()
		class UMaterialInstance* MapMat4;
	UPROPERTY()
		class USoundCue* BackgroundMusic;
	UPROPERTY()
		class UAudioComponent* AudioControll;

	void BackMusicOn();
	void BackMusicOff();

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION(BlueprintCallable)
	void StartButtonCallback();
	UFUNCTION(BlueprintCallable)
	void ExitButtonCallback();
	UFUNCTION(BlueprintCallable)
	void GameSelectButtonCallback();
	UFUNCTION(BlueprintCallable)
	void Stage1ButtonCallback();
	UFUNCTION(BlueprintCallable)
	void Stage2ButtonCallback();
	UFUNCTION(BlueprintCallable)
	void Stage3ButtonCallback();
	UFUNCTION(BlueprintCallable)
	void BackButtonCallback();
	UFUNCTION(BlueprintCallable)
	void Stage1ButtonHovered();
	UFUNCTION(BlueprintCallable)
	void Stage2ButtonHovered();
	UFUNCTION(BlueprintCallable)
	void Stage3ButtonHovered();
	UFUNCTION(BlueprintCallable)
	void Stage1ButtonOffHovered();
	UFUNCTION(BlueprintCallable)
	void Stage2ButtonOffHovered();
	UFUNCTION(BlueprintCallable)
	void Stage3ButtonOffHovered();
	void DeleteSave();
};
