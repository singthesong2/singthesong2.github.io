// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "ARBase.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API AARBase : public AGunBase
{
	GENERATED_BODY()

public:
	AARBase();
	
public:
	UPROPERTY()
	ESelectorType Selector;

	UPROPERTY()
	int BurstFireCount;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void SelectorChange();
};
