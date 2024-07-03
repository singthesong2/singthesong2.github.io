// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerWidget.h"
#include "Components/TextBlock.h"
#include "Runtime/UMG/Public/UMG.h"

//#include "Animation/WidgetAnimation.h"

void UTimerWidget::UpdateTexts(int Min, int Sec, int MilSec)
{
	MinText->SetText(GetTextFronInt(Min));
	SecText->SetText(GetTextFronInt(Sec));
	MicText->SetText(GetTextFronInt(MilSec));
	UE_LOG(LogTemp, Log, TEXT("%d:%d:%d"), Min, Sec, MilSec);
}

void UTimerWidget::CalcultateTime()
{
	int min = (int)LeftTime / 60;
	int sec = (int)LeftTime - (min * 60);
	int milsec = (LeftTime - (int)LeftTime) * 100;
	if (sec < FinalLimit)
	{
		WarningStage = WARN_STAGE::FINAL;
	}
	else if (sec < FirstLimit)
	{
		WarningStage = WARN_STAGE::FIRST;
	}
	else
	{
		WarningStage = WARN_STAGE::NONE;
	}
	UpdateTexts(min, sec, milsec);
}

void UTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (false == IsOnTimer) return;
	LeftTime -= InDeltaTime;
	CalcultateTime();
	if (0.f > LeftTime)
	{
		UE_LOG(LogTemp, Log, TEXT("Time is over"));
		IsOnTimer = false;
		_IsTimeOver = true;
		SetVisibility(ESlateVisibility::Collapsed);
		//Send timer over message to other if need	

	}
	if (true == IsWarningStageModified())
	{
		HandleWarningAnimation();
	}
}

void UTimerWidget::ExecuteTimer(const float& InitialTime, const int& _FirstLimit, const int& _FinalLimit, const bool& IsUsingWarning)
{
	InitWidgetState(InitialTime, _FirstLimit, _FinalLimit, IsUsingWarning);
}

void UTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();	
	SetWidgetAnimation();
}

void UTimerWidget::InitWidgetState(const float& InitialTime, const int& _FirstLimit, const int& _FinalLimit, const bool& IsUsingWarning)
{
	LeftTime = InitialTime;
	IsOnTimer = true;
	FirstLimit = _FirstLimit;
	FinalLimit = _FinalLimit;
	_IsTimeOver = false;
	WarningStage = WARN_STAGE::NONE;
	CurrentWarningStage = WARN_STAGE::NONE;
	//IsOnWarning = false;
	WarningBool = IsUsingWarning;
}

inline FText UTimerWidget::GetTextFronInt(int Num)
{
	return FText::FromString(FString::Printf(TEXT("%02d"), Num));
}

inline bool UTimerWidget::IsWarningStageModified()
{
	return WarningStage != CurrentWarningStage;
}

inline void UTimerWidget::UpdateWarningStage()
{
	CurrentWarningStage = WarningStage;
}

void UTimerWidget::HandleWarningAnimation()
{

	switch (WarningStage)
	{
		case WARN_STAGE::NONE:	
			UpdateWarningStage();
			UE_LOG(LogTemp, Log, TEXT("Stop playing warning animation"));
			PlayWarningAnimation(0.f);
			break;
		case WARN_STAGE::FIRST:
			UpdateWarningStage();
			PlayWarningAnimation(SlowWarningRate);
			UE_LOG(LogTemp, Log, TEXT("Playing warning animation 1"));
			break;
		case WARN_STAGE::FINAL:
			UpdateWarningStage();
			PlayWarningAnimation(FastWarningRate);
			UE_LOG(LogTemp, Log, TEXT("Playing warning animation 2"));
			break;
	}
}

void UTimerWidget::PlayWarningAnimation(float Rate)
{
	if (false == WarningBool)
	{
		UE_LOG(LogTemp, Log, TEXT("Escape"));
		return;
	}
	if (0.f == Rate) 
	{
		StopAnimation(WarningAnimation);
		UE_LOG(LogTemp, Log, TEXT("Stop"));
	}
	else 
	{
		PlayAnimation(WarningAnimation, 0.f, 10, EUMGSequencePlayMode::Forward, Rate, true);
		UE_LOG(LogTemp, Log, TEXT("Play"));
	}
}

void UTimerWidget::SetWidgetAnimation()
{
	auto WidgetClass = GetWidgetTreeOwningClass();
	AnimationMap.Empty();
	for (auto& Anim : WidgetClass->Animations)
	{
		AnimationMap.Add(Anim.GetName(), Anim);
	}
	if (!AnimationMap.IsEmpty())
	{
		WarningAnimation = AnimationMap[FString("Warning_Inst")];
		if (nullptr == WarningAnimation) UE_LOG(LogTemp, Log, TEXT("Failed to set animation"));
	}
}

UWidgetAnimation* UTimerWidget::GetAnimationByName(const FName& Name) const
{
	return nullptr;
}

bool UTimerWidget::IsTimeOver()
{
	return _IsTimeOver;
}
