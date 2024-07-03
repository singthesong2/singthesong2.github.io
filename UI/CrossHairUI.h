// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrossHairUI.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API UCrossHairUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (Bindwidget))
	class UBorder* Bottom;

	UPROPERTY(BlueprintReadWrite, meta = (Bindwidget))
	class UBorder* Top;

	UPROPERTY(BlueprintReadWrite, meta = (Bindwidget))
	class UBorder* Left;

	UPROPERTY(BlueprintReadWrite, meta = (Bindwidget))
	class UBorder* Right;

	UPROPERTY(BlueprintReadWrite, meta = (Bindwidget))
	class UBorder* Hit;

	UPROPERTY(BlueprintReadWrite)
	float Spread = 10;

	UPROPERTY(BlueprintReadWrite)
	float Thickness = 6;

	UPROPERTY(BlueprintReadWrite)
	float Length = 20;

	UPROPERTY(BlueprintReadWrite)
	bool Visible;

	float MaxSpread = 50;
	float MinSpread = 10;

public:
	void SetCrossHairVisible(bool _flag);
};
