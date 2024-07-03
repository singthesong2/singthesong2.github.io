// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "CustomEnum.h"
#include "Widget_AI_Reaction.generated.h"

UCLASS()
class SPECTER_API UWidget_AI_Reaction : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void IconPlay(E_EnemyState State);
};
