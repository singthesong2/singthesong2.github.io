// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Event/DialogueTable.h"
#include "DialogueWidget.generated.h"
/**
 * 
 */
UCLASS()
class SPECTER_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	TMap<SpeakerType, FString> Speaker;
	UPROPERTY(EditAnywhere, Category = "Text setting")
	TArray<FString> SpeakerName;
	UPROPERTY(EditAnywhere, Category = "Text setting")
	TArray<FColor> TextColor;
	TArray<class UTextBlock*> TextBlocks;
	void SetSpeakerNames();
	void AddSpeakerNames();
	void SetTextBlocks();
	void UpdateTextColor(const SpeakerType& _Speaker);
public:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* NameLog;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Colon;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextLog;
	void UpdateDialogue(const SpeakerType& _Speaker, const FString& Text);

};
