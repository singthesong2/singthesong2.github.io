// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetCover.generated.h"

UCLASS()
class SPECTER_API UBTTask_GetCover : public UBTTaskNode {
	GENERATED_BODY()

public:
	UBTTask_GetCover();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector Point;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
