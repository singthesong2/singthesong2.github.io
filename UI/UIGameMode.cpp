// Fill out your copyright notice in the Description page of Project Settings.


#include "UIGameMode.h"
#include "UI/PauseUI.h"
#include "DamageUI.h"
#include "UI/StopPauseUI.h"
#include "UObject/ConstructorHelpers.h"
#include "DialogueWidget.h"
//#include "../Event/DialogueInstance.h"
//#include "Kismet/GameplayStatics.h"
#include "TimerWidget.h"
#include "UIMainHUD.h"
#include "UI/StageLoadingHUD.h"
#include "CrossHairUI.h"
#include "DialogueTable.h"
#include "DialogueInstance.h"

AUIGameMode::AUIGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/WeaponSystem/Player/BP_Player2.BP_Player2_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Script/Engine.Blueprint'/Game/Player/BP_Player_Controller.BP_Player_Controller_C'"));
	if (PlayerControllerBPClass.Class != NULL) {
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// 블루프린트 클래스를 받아온다
	static ConstructorHelpers::FClassFinder<UUIMainHUD> MainHUDWidgetAsset(TEXT("/Game/UI/WBPUI.WBPUI_C"));
	static ConstructorHelpers::FClassFinder<UDamageUI> DamageHUDWidgetAsset(TEXT("/Game/UI/WBPDUI.WBPDUI_C"));
	//static ConstructorHelpers::FClassFinder<UUserWidget> CrossHairAsset(TEXT("/Game/UI/WBCrosshair.WBCrosshair_C"));
	static ConstructorHelpers::FClassFinder<UCrossHairUI> CrossHairAsset(TEXT("/Game/UI/WBCrosshair_2.WBCrosshair_2_C"));

	static ConstructorHelpers::FClassFinder<UPauseUI> PauseHUDWidgetAsset(TEXT("/Game/UI/WBPPUI.WBPPUI_C"));
	static ConstructorHelpers::FClassFinder<UStopPauseUI> StopPauseHUDWidgetAsset(TEXT("/Game/UI/WBPSTOPUI.WBPSTOPUI_C"));
	static ConstructorHelpers::FClassFinder<UDialogueWidget>DialogueHUDAsset(TEXT("/Game/UI/WBP_Dialogue.WBP_Dialogue_C"));

	static ConstructorHelpers::FClassFinder<UUserWidget> SniperWidgetAsset(TEXT("/Game/WeaponSystem/CrossHair/UW_SniperScope.UW_SniperScope_C"));

	static ConstructorHelpers::FClassFinder<UUserWidget> CompassbarWidgetAsset(TEXT("/Game/UI/BP/WBP_CompassBar.WBP_CompassBar_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> TimerWidgetAsset(TEXT("/Game/WeaponSystem/Dialouge/WBP_Timer.WBP_Timer_C"));

	
	static ConstructorHelpers::FClassFinder<UUserWidget> AmsalWidgetAsset(TEXT("/Game/WeaponSystem/WIdget/WD_Amsal.WD_Amsal_C"));

	static ConstructorHelpers::FClassFinder<UUserWidget> GuideWidgetAsset(TEXT("/Game/UI/WBP_KeyGuide.WBP_KeyGuide_C"));

	static ConstructorHelpers::FClassFinder<UUserWidget> StageLoadingHUDWidgetAsset(TEXT("/Game/UI/WBP_StageLoading.WBP_StageLoading_C"));

	// TSubclassOf 템플릿 클래스 객체에 블루프린트 클래스를 넣어준다
	if (MainHUDWidgetAsset.Succeeded())
	{
		MainHUDWidgetClass = MainHUDWidgetAsset.Class;
	}
	if (DamageHUDWidgetAsset.Succeeded())
	{
		DamageHUDWidgetClass = DamageHUDWidgetAsset.Class;
	}
	if (PauseHUDWidgetAsset.Succeeded())
	{
		PauseHUDWidgetClass = PauseHUDWidgetAsset.Class;
	}
	if (StopPauseHUDWidgetAsset.Succeeded())
	{
		StopPauseHUDWidgetClass = StopPauseHUDWidgetAsset.Class;
	}
	if (CrossHairAsset.Succeeded())
	{
		CrossHairWidgetclass = CrossHairAsset.Class;
	}

	if (DialogueHUDAsset.Succeeded())
	{
		DialogueWidgetClass = DialogueHUDAsset.Class;
	}

	if (SniperWidgetAsset.Succeeded())
	{
		SniperWidgetclass = SniperWidgetAsset.Class;
	}
	UE_LOG(LogTemp, Log, TEXT("%d"), DialogueHUDAsset.Class == nullptr ? 0 : 1);

	if (CompassbarWidgetAsset.Succeeded())
	{
		CompassbarWidgetclass = CompassbarWidgetAsset.Class;
	}

	if (AmsalWidgetAsset.Succeeded())
	{
		AmsalWidgetclass = AmsalWidgetAsset.Class;
	}	
	
	if (GuideWidgetAsset.Succeeded())
	{
		GuideWidgetclass = GuideWidgetAsset.Class;
	}

	if (TimerWidgetAsset.Succeeded())
	{
		TimerWidgetClass = TimerWidgetAsset.Class;
	}

	if (StageLoadingHUDWidgetAsset.Succeeded())
	{
		StageLoadingHUDWidgetClass = StageLoadingHUDWidgetAsset.Class;
	}
	//if(St)
	//if (false == DialogueHUDAsset.Class)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Invalid MAINHUD class"));

	//}
}

void AUIGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(DamageHUDWidgetClass))
	{
		DamageHUDWidget = Cast<UDamageUI>(CreateWidget(GetWorld(), DamageHUDWidgetClass));

		if (IsValid(DamageHUDWidget))
		{
			// 위젯을 뷰포트에 띄우는 함수
			DamageHUDWidget->AddToViewport();
		}
	}

	if (IsValid(MainHUDWidgetClass))
	{
		MainHUDWidget = Cast<UUIMainHUD>(CreateWidget(GetWorld(), MainHUDWidgetClass));

		if (IsValid(MainHUDWidget))
		{
			// 위젯을 뷰포트에 띄우는 함수
			MainHUDWidget->AddToViewport();
		}
	}

	if (IsValid(CrossHairWidgetclass))
	{
		CrossHairWidget = Cast<UCrossHairUI>(CreateWidget(GetWorld(), CrossHairWidgetclass));

		if (IsValid(CrossHairWidget))
		{
			CrossHairWidget->AddToViewport();
			CrossHairWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (IsValid(CompassbarWidgetclass))
	{
		CompassbarWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), CompassbarWidgetclass));
			// 위젯을 뷰포트에 띄우는 함수
		if (IsValid(CompassbarWidget))
		{
			CompassbarWidget->AddToViewport();
		}
	}

	if (IsValid(AmsalWidgetclass))
	{
		AmsalWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), AmsalWidgetclass));
		// 위젯을 뷰포트에 띄우는 함수
		if (IsValid(AmsalWidget))
		{
			AmsalWidget->AddToViewport();
			AmsalWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}	
	
	if (IsValid(GuideWidgetclass))
	{
		GuideWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), GuideWidgetclass));
		// 위젯을 뷰포트에 띄우는 함수
		if (IsValid(GuideWidget))
		{
			GuideWidget->AddToViewport();
			//AmsalWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (false == IsValid(MainHUDWidgetClass))
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid HUD class"));
		return;
	}
	DialogueWidget = Cast<UDialogueWidget>(CreateWidget(GetWorld(), DialogueWidgetClass));
	if (false == IsValid(DialogueWidget))
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid HUD Widget"));
		return;
	}
	DialogueWidget->AddToViewport();

	if (IsValid(SniperWidgetclass))
	{
		SniperWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), SniperWidgetclass));

		if (IsValid(SniperWidget))
		{
			SniperWidget->AddToViewport();
			SniperWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}	
	if (IsValid(TimerWidgetClass))
	{
		TimerWidget = Cast<UTimerWidget>(CreateWidget(GetWorld(), TimerWidgetClass));
		if (IsValid(TimerWidget))
		{
			TimerWidget->AddToViewport();
			TimerWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	//UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	//inst->LoadJson();

	if (IsValid(PauseHUDWidgetClass))
	{
		PauseHUDWidget = Cast<UPauseUI>(CreateWidget(GetWorld(), PauseHUDWidgetClass));

		if (IsValid(PauseHUDWidget))
		{
			PauseHUDWidget->AddToViewport();
		}
	}

	if (IsValid(StopPauseHUDWidgetClass))
	{
		StopPauseHUDWidget = Cast<UStopPauseUI>(CreateWidget(GetWorld(), StopPauseHUDWidgetClass));

		if (IsValid(StopPauseHUDWidget))
		{
			StopPauseHUDWidget->AddToViewport();
		}
	}

	if (IsValid(StageLoadingHUDWidgetClass))
	{
		StageLoadingHUDWidget = Cast<UStageLoadingHUD>(CreateWidget(GetWorld(), StageLoadingHUDWidgetClass));

		if (IsValid(StageLoadingHUDWidget))
		{
			StageLoadingHUDWidget->AddToViewport();
			LoadingHUD();
		}
	}
}

void AUIGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this))->ResetActors();
}

void AUIGameMode::UpdateDialogueWidget(const SpeakerType& Speaker, const FString& Text)
{
	DialogueWidget->UpdateDialogue(Speaker, Text);
	DialogueWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void AUIGameMode::HideDialogueWidget()
{
	DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
}


void AUIGameMode::SetTimerWidget(const float& InitialTime, const int& FirstLimit, const int& FinalLimit, const bool& IsUsingWarning)
{
	TimerWidget->ExecuteTimer(InitialTime, FirstLimit, FinalLimit, IsUsingWarning);
	if (TimerWidget->GetVisibility() == ESlateVisibility::Collapsed) 
	{
		TimerWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		TimerWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AUIGameMode::HideTimerWidget()
{
	DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AUIGameMode::LoadingHUD()
{
	StageLoadingHUDWidget->StageLoadOn();
}

void AUIGameMode::StageToStage()
{
	StageLoadingHUDWidget->AddToViewport();
	StageLoadingHUDWidget->StageLoadOn();
}