// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageUI.h"

#include "Components/Image.h"

void UDamageUI::NativeConstruct()
{
	Super::NativeConstruct();

	Damage1Impact = Cast<UImage>(GetWidgetFromName(TEXT("Damage1Impact")));
	Damage2Impact = Cast<UImage>(GetWidgetFromName(TEXT("Damage2Impact")));
	//DamageImpact->ColorAndOpacity = { 255.f, 0.f, 0.f, color };

}

void UDamageUI::SetOp(float fOpacity)
{
	if (IsValid(Damage1Impact))
		Damage1Impact->SetOpacity(fOpacity);
}

void UDamageUI::SetVisi()
{
	Damage1Impact->SetVisibility(ESlateVisibility::Visible);
	StartPlayAnimation();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UDamageUI::EndVisi, 2.0f, false);
}

void UDamageUI::EndVisi()
{
	Damage1Impact->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UDamageUI::StopPlayAnimation, .1f, false);
}

void UDamageUI::StartPlayAnimation()
{
	PlayAnimation(DamageOpacity);
}

void UDamageUI::StopPlayAnimation()
{
	StopAnimation(DamageOpacity);
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UDamageUI::EventPlayAnimation, .1f, false);
}

void UDamageUI::EventPlayAnimation()
{
	DamageOpacityEvent.Clear();
	DamageOpacityEvent.BindUFunction(this, FName(FString(TEXT("DamageOpacity"))));
	BindToAnimationFinished(DamageOpacity, DamageOpacityEvent);
	//PlayAnimation(DamageOpacity);
}




void UDamageUI::SetVisi2()
{
	Damage2Impact->SetVisibility(ESlateVisibility::Visible);
	StartPlayAnimation2();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UDamageUI::EndVisi2, 2.0f, false);
}

void UDamageUI::EndVisi2()
{
	Damage2Impact->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UDamageUI::StopPlayAnimation2, .1f, false);
}

void UDamageUI::StartPlayAnimation2()
{
	PlayAnimation(DamageOpacity2);
}

void UDamageUI::StopPlayAnimation2()
{
	StopAnimation(DamageOpacity2);
	//GetWorld()->GetTimerManager().SetTimer(timer, this, &UDamageUI::EventPlayAnimation, .1f, false);
}