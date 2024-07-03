// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"
#include "../Event/DialogueTable.h"
#include "Components/TextBlock.h"	

void UDialogueWidget::SetSpeakerNames()
{
	Speaker = TMap<SpeakerType, FString>();
	AddSpeakerNames();
}

void UDialogueWidget::AddSpeakerNames()
{
	for (int i = 0; i < SpeakerName.Num(); i++)
	{
		Speaker.Add((SpeakerType)i, SpeakerName[i]);
	}
}

void UDialogueWidget::SetTextBlocks()
{
	TextBlocks = TArray<UTextBlock*>();
	TextBlocks.Add(NameLog);
	TextBlocks.Add(Colon);
	TextBlocks.Add(TextLog);
}

void UDialogueWidget::UpdateTextColor(const SpeakerType& _Speaker)
{
	//for (auto& text : TextBlocks)
	//{
	//	text->SetColorAndOpacity(TextColor[(int)_Speaker]);
	//}
	NameLog->SetColorAndOpacity(TextColor[(int)_Speaker]);
}

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetSpeakerNames();
	SetTextBlocks();
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UDialogueWidget::UpdateDialogue(const SpeakerType& _Speaker, const FString& Text)
{
	UE_LOG(LogTemp, Log, TEXT("Widget updated"));
	if (NameLog != nullptr)
	{
		NameLog->SetText(FText::FromString(Speaker[_Speaker]));
	}
	if (TextLog != nullptr)
	{
		TextLog->SetText(FText::FromString(Text));
	}
	UpdateTextColor(_Speaker);
}

//void UDialogueWidget::UpdateDialogue(const FString& Text)
//{
//	UE_LOG(LogTemp, Log, TEXT("Widget updated"));
//	if (TextLog != nullptr)
//	{
//		TextLog->SetText(FText::FromString(Text));
//	}
//	else
//	{
//		UE_LOG(LogTemp, Log, TEXT("Textbox is nullptr"));
//	}
//}
