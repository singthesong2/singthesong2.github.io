// Fill out your copyright notice in the Description page of Project Settings.

#include "StopPauseUI.h"
#include "Event/DialogueInstance.h"


void UStopPauseUI::NativeConstruct()
{
	Super::NativeConstruct();

	Pause= Cast<UTextBlock>(GetWidgetFromName(TEXT("Pause")));
	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("ResumeButton")));
	Resume = Cast<UTextBlock>(GetWidgetFromName(TEXT("Resume")));
	NewGameButton = Cast<UButton>(GetWidgetFromName(TEXT("NewGameButton")));
	NewGame = Cast<UTextBlock>(GetWidgetFromName(TEXT("NewGame")));
	LoadGameButton = Cast<UButton>(GetWidgetFromName(TEXT("LoadGameButton")));
	LoadGame = Cast<UTextBlock>(GetWidgetFromName(TEXT("LoadGame")));
	ExitGameButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitGameButton")));
	ExitGame = Cast<UTextBlock>(GetWidgetFromName(TEXT("ExitGame")));
	Background = Cast<UBackgroundBlur>(GetWidgetFromName(TEXT("Background")));
	TitleLogo = Cast<UImage>(GetWidgetFromName(TEXT("TitleLogo")));

	ResumeButton->OnClicked.AddDynamic(this, &UStopPauseUI::ResumeButtonCallback);
	NewGameButton->OnClicked.AddDynamic(this, &UStopPauseUI::NewGameButtonCallback);
	LoadGameButton->OnClicked.AddDynamic(this, &UStopPauseUI::LoadGameButtonCallback);
	ExitGameButton->OnClicked.AddDynamic(this, &UStopPauseUI::ExitGameButtonCallback);
}


void UStopPauseUI::OptionOn()
{
	MouseOn = true;
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	Pause->SetVisibility(ESlateVisibility::Visible);
	ResumeButton->SetVisibility(ESlateVisibility::Visible);
	Resume->SetVisibility(ESlateVisibility::Visible);
	NewGameButton->SetVisibility(ESlateVisibility::Visible);
	NewGame->SetVisibility(ESlateVisibility::Visible);
	LoadGameButton->SetVisibility(ESlateVisibility::Visible);
	LoadGame->SetVisibility(ESlateVisibility::Visible);
	ExitGameButton->SetVisibility(ESlateVisibility::Visible);
	ExitGame->SetVisibility(ESlateVisibility::Visible);
	Background->SetVisibility(ESlateVisibility::Visible);
	TitleLogo->SetVisibility(ESlateVisibility::Visible);
}

void UStopPauseUI::OptionOff()
{
	MouseOn = false;
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	Pause->SetVisibility(ESlateVisibility::Hidden);
	ResumeButton->SetVisibility(ESlateVisibility::Hidden);
	Resume->SetVisibility(ESlateVisibility::Hidden);
	NewGameButton->SetVisibility(ESlateVisibility::Hidden);
	NewGame->SetVisibility(ESlateVisibility::Hidden);
	LoadGameButton->SetVisibility(ESlateVisibility::Hidden);
	LoadGame->SetVisibility(ESlateVisibility::Hidden);
	ExitGameButton->SetVisibility(ESlateVisibility::Hidden);
	ExitGame->SetVisibility(ESlateVisibility::Hidden);
	Background->SetVisibility(ESlateVisibility::Hidden);
	TitleLogo->SetVisibility(ESlateVisibility::Hidden);
}

void UStopPauseUI::ResumeButtonCallback()
{
	MouseOn = false;
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	Pause->SetVisibility(ESlateVisibility::Hidden);
	ResumeButton->SetVisibility(ESlateVisibility::Hidden);
	Resume->SetVisibility(ESlateVisibility::Hidden);
	NewGameButton->SetVisibility(ESlateVisibility::Hidden);
	NewGame->SetVisibility(ESlateVisibility::Hidden);
	LoadGameButton->SetVisibility(ESlateVisibility::Hidden);
	LoadGame->SetVisibility(ESlateVisibility::Hidden);
	ExitGameButton->SetVisibility(ESlateVisibility::Hidden);
	ExitGame->SetVisibility(ESlateVisibility::Hidden);
	Background->SetVisibility(ESlateVisibility::Hidden);
	TitleLogo->SetVisibility(ESlateVisibility::Hidden);
}

void UStopPauseUI::NewGameButtonCallback()
{
	RemoveFromParent();
	
	// Stage 1
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Demo_P"))
	{
		FLatentActionInfo LActionInfo;
		UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Demo_P"), true, true, LActionInfo);
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo_P"));
	}
	// Stage 2
	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Demo_P_Level02_Main"))
	{
		FLatentActionInfo LActionInfo;
		UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Demo_P_Level02_Main"), true, true, LActionInfo);
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo_P_Level02_Main"));
	}
	// Stage 3
	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Demo_P_Level03_Main"))
	{
		FLatentActionInfo LActionInfo;
		UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Demo_P_Level03_Main"), true, true, LActionInfo);
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo_P_Level03_Main"));
	}
}

void UStopPauseUI::LoadGameButtonCallback()
{
	UDialogueInstance* Loading = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	if (nullptr == Loading)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to get game instance"));
		return;
	}
	Loading->LoadJson();
	ResumeButtonCallback();
}

void UStopPauseUI::ExitGameButtonCallback()
{
	RemoveFromParent();
	FLatentActionInfo LActionInfo;
	UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Start"), true, true, LActionInfo);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}

void UStopPauseUI::DeleteSave()
{
	UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	inst->DeleteSaveData();
}