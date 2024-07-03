// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_ComebackFinish.generated.h"

UCLASS()
class SPECTER_API UTask_BT_ComebackFinish : public UBTTaskNode {
	GENERATED_BODY()

public:
	 UTask_BT_ComebackFinish();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
