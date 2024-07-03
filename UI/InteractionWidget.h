// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (Bindwidget))
	FText ItemName = FText::FromName(FName(TEXT("DDDD")));

	void SetItemName(FName _newName) { ItemName = FText::FromName(_newName); }
};
