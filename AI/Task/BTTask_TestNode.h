// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TestNode.generated.h"

UCLASS()
class SPECTER_API UBTTask_TestNode : public UBTTaskNode {
	GENERATED_BODY()

public:
	UBTTask_TestNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
