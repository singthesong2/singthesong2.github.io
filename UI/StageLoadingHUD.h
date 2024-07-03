// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "StageLoadingHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API UStageLoadingHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UStageLoadingHUD(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* StageBackLoad;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* MapImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* StageText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* StageName;
	UPROPERTY()
		class UTexture2D* MapText1;
	UPROPERTY()
		class UTexture2D* MapText2;
	UPROPERTY()
		class UTexture2D* MapText3;
	UPROPERTY()
		FTimerHandle timer;

	void StageLoadOn();
	void StageLoadOff();

protected:
	virtual void NativeConstruct() override;
};
