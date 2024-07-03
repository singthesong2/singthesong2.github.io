// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Event/DialogueTable.h"
#include "GameFramework/GameModeBase.h"
#include "UIGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API AUIGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AUIGameMode();
	class UUIMainHUD* GetMainHUDWidget() const
	{
		return MainHUDWidget;
	}
	class UDamageUI* GetDamageHUDWidget() const
	{
		return DamageHUDWidget;
	}
	class UPauseUI* GetPauseHUDWidget() const
	{
		return PauseHUDWidget;
	}
	class UStopPauseUI* GetStopPauseHUDWidget() const
	{
		return StopPauseHUDWidget;
	}
	class UCrossHairUI* GetCrossHairWidget() const
	{
		return CrossHairWidget;
	}
	class UUserWidget* GetSniperWidget() const
	{
		return SniperWidget;
	}
	class UUserWidget* GetCompassbarWidgetclass() const
	{
		return CompassbarWidget;
	}
	class UUserWidget* GetAmsalWidgetclass() const
	{
		return AmsalWidget;
	}
	class UUserWidget* GetGuideWidgetclass() const
	{
		return GuideWidget;
	}

	class UStageLoadingHUD* GetStageLoadingHUDWidget() const
	{
		return StageLoadingHUDWidget;
	}

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> MainHUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> DamageHUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> PauseHUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> StopPauseHUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> CrossHairWidgetclass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> SniperWidgetclass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> CompassbarWidgetclass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<UUserWidget> TimerWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> DialogueWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> AmsalWidgetclass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> GuideWidgetclass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<class UUserWidget> StageLoadingHUDWidgetClass;

	UPROPERTY()
	class UUIMainHUD* MainHUDWidget;
	UPROPERTY()
	class UDamageUI* DamageHUDWidget;
	UPROPERTY()
	class UPauseUI* PauseHUDWidget;
	UPROPERTY()
	class UStopPauseUI* StopPauseHUDWidget;
	UPROPERTY()
	class UCrossHairUI* CrossHairWidget;
	UPROPERTY()
	class UUserWidget* SniperWidget;
	UPROPERTY()
	class UStageLoadingHUD* StageLoadingHUDWidget;

	UPROPERTY()
	class UUserWidget* CompassbarWidget;
	UPROPERTY()
	class UUserWidget* AmsalWidget;	
	UPROPERTY()
	class UUserWidget* GuideWidget;

	UPROPERTY()
	class UDialogueWidget* DialogueWidget;

	UPROPERTY()
	class UTimerWidget* TimerWidget;

	/*UPROPERTY()
	class UMarkerMeter* BorderLineMarker;
	UPROPERTY(VisibleAnywhere)	
	TArray<UMarkerMeter*> CurrentActivedMarkers;
	void AddActivedMarker(UMarkerMeter*);*/
	void LoadingHUD();
	void StageToStage();
public:
	void UpdateDialogueWidget(const SpeakerType& Speaker, const FString& Text);
	void HideDialogueWidget();
	void SetTimerWidget(const float& InitialTime, const int& FirstLimit, const int& FinalLimit, const bool& IsUsingWarning);
	void HideTimerWidget();
};
