// Fill out your copyright notice in the Description page of Project Settings.


#include "ARBase.h"

AARBase::AARBase()
{
	WeaponSlot = EWeaponSlot::WeaponSlot2;
}


void AARBase::BeginPlay()
{
	Super::BeginPlay();
}

void AARBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARBase::SelectorChange()
{
	Selector = ESelectorType(((int)Selector + 1) % (int)ESelectorType::End);
}
