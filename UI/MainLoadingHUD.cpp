// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainLoadingHUD.h"

void UMainLoadingHUD::NativeConstruct()
{
	Super::NativeConstruct();

	LoadingHUD = Cast<UImage>(GetWidgetFromName(TEXT("LoadingHUD")));
	LoadingText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LoadingText")));
}