// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_CheckCover.generated.h"

UCLASS()
class SPECTER_API UTask_BT_CheckCover : public UBTTaskNode {
	GENERATED_BODY()

public:
	UTask_BT_CheckCover();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
