// Fill out your copyright notice in the Description page of Project Settings.


#include "MarkerWidget.h"
#include "Components/TextBlock.h"

void UMarkerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	this->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMarkerWidget::UpdateDistance(const double& Dist)
{
	Marker_Text->SetText(FText::FromString(FString::Printf(TEXT("%dm"), (int)(Dist * 0.01))));
}
