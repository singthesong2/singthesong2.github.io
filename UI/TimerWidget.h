// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	enum WARN_STAGE
	{
		NONE = 0,
		FIRST,
		FINAL,
	};
protected:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool WarningBool;
	UPROPERTY(BlueprintReadWrite)
	int FirstLimit;
	UPROPERTY(BlueprintReadWrite)
	int FinalLimit;
	UPROPERTY(BlueprintReadWrite)
	float LeftTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float SlowWarningRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float FastWarningRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MinText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SecText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MicText; 
	TMap<FString, UWidgetAnimation*> AnimationMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UWidgetAnimation* WarningAnimation;
	bool IsOnTimer = false;
	bool _IsTimeOver = false;
	//bool IsOnWarning = false;
	int WarningStage;
	int CurrentWarningStage;
	void InitWidgetState(const float& InitialTime, const int& _FirstLimit, const int& _FinalLimit, const bool& IsUsingWarning);
	void CalcultateTime();
	void UpdateTexts(int Min, int Sec, int MilSec);
	inline FText GetTextFronInt(int Num);
	inline bool IsWarningStageModified();
	inline void UpdateWarningStage();
	void HandleWarningAnimation();
	void PlayWarningAnimation(float Rate);
	void SetWidgetAnimation();
	UWidgetAnimation* GetAnimationByName(const FName& Name) const;
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void ExecuteTimer(const float& InitialTime, const int& _FirstLimit, const int& _FinalLimit, const bool& IsUsingWarning);
	bool IsTimeOver();
};
