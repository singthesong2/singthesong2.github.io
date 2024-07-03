// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetSplineLocation.generated.h"

UCLASS()
class SPECTER_API UBTTask_GetSplineLocation : public UBTTaskNode {
	GENERATED_BODY()

public:
	UBTTask_GetSplineLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
