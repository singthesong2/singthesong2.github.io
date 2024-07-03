// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_TurnPoint.generated.h"

UCLASS()
class SPECTER_API UTask_BT_TurnPoint : public UBTTaskNode {
	GENERATED_BODY()

public:
	UTask_BT_TurnPoint();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector Point;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float PlayRate;
	//FBlackboardKeySelector PlayRate;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
