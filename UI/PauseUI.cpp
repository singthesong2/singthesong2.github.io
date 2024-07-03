// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseUI.h"
#include "../Event/DialogueInstance.h"
#include "Saveload/SaveSystem.h"

void UPauseUI::NativeConstruct()
{
	Super::NativeConstruct();

	GameOver= Cast<UTextBlock>(GetWidgetFromName(TEXT("GameOver")));
	ReStartButton= Cast<UButton>(GetWidgetFromName(TEXT("ReStartButton")));
	NewStartButton = Cast<UButton>(GetWidgetFromName(TEXT("NewStartButton")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));
	ReStart= Cast<UTextBlock>(GetWidgetFromName(TEXT("ReStart")));
	NewStart = Cast<UTextBlock>(GetWidgetFromName(TEXT("NewStart")));
	Exit = Cast<UTextBlock>(GetWidgetFromName(TEXT("Exit")));
	Background = Cast<UBackgroundBlur>(GetWidgetFromName(TEXT("Background")));
	GameOverImage = Cast<UImage>(GetWidgetFromName(TEXT("OverImage")));

	ReStartButton->OnClicked.AddDynamic(this, &UPauseUI::ReStartButtonCallback);
	NewStartButton->OnClicked.AddDynamic(this, &UPauseUI::NewStartButtonCallback);
	ExitButton->OnClicked.AddDynamic(this, &UPauseUI::ExitButtonCallback);
}

void UPauseUI::DeadPauseOn()
{
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UPauseUI::Pause, 0.1f, false);
}


void UPauseUI::Pause()
{
	PlayAnimation(BackBlur);
	MouseOnOff = true;
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%s"), MouseOnOff ? TEXT("True") : TEXT("False")));
	/*GameOver->SetVisibility(ESlateVisibility::Visible);
	ReStartButton->SetVisibility(ESlateVisibility::Visible);
	NewStartButton->SetVisibility(ESlateVisibility::Visible);
	ExitButton->SetVisibility(ESlateVisibility::Visible);
	ReStart->SetVisibility(ESlateVisibility::Visible);
	NewStart->SetVisibility(ESlateVisibility::Visible);
	Exit->SetVisibility(ESlateVisibility::Visible);
	Background->SetVisibility(ESlateVisibility::Visible);*/
	Background->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UPauseUI::StopBlur, 2.0f, false);
}

void UPauseUI::ReStartButtonCallback()
{
	UDialogueInstance* ReStartLoad = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	if (nullptr == ReStartLoad)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to get game instance"));
		return;
	}
	RemoveFromParent();
	ReStartLoad->LoadJson();
	FLatentActionInfo LActionInfo;
	UGameplayStatics::LoadStreamLevel(this, FName(*GetWorld()->GetName()), true, true, LActionInfo);
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void UPauseUI::NewStartButtonCallback()
{
	MouseOnOff = false;
	RemoveFromParent();
	// Stage 1
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Demo_P"))
	{
		DeleteSave();
		FLatentActionInfo LActionInfo;
		UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Demo_P"), true, true, LActionInfo);
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo_P"));
	}
	// Stage 2
	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Demo_P_Level02_Main"))
	{
		DeleteSave();
		FLatentActionInfo LActionInfo;
		UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Demo_P_Level02_Main"), true, true, LActionInfo);
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo_P_Level02_Main"));
	}
	// Stage 3
	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Demo_P_Level03_Main"))
	{
		DeleteSave();
		FLatentActionInfo LActionInfo;
		UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Demo_P_Level03_Main"), true, true, LActionInfo);
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo_P_Level03_Main"));
	}
}

void UPauseUI::ExitButtonCallback()
{
	MouseOnOff = false;
	RemoveFromParent();
	DeleteSave();
	FLatentActionInfo LActionInfo;
	UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Start"), true, true, LActionInfo);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}

void UPauseUI::StopBlur()
{
	//StopAnimation(BackBlur);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	GameOverImage->SetVisibility(ESlateVisibility::Visible);
	ReStartButton->SetVisibility(ESlateVisibility::Visible);
	//NewStartButton->SetVisibility(ESlateVisibility::Visible);
	ExitButton->SetVisibility(ESlateVisibility::Visible);
	ReStart->SetVisibility(ESlateVisibility::Visible);
	NewStart->SetVisibility(ESlateVisibility::Visible);
	Exit->SetVisibility(ESlateVisibility::Visible);
}

void UPauseUI::DeleteSave()
{
	UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	inst->DeleteSaveData();
}
