// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIEnum.h"
#include "Task_ChangeState.generated.h"

UCLASS()
class SPECTER_API UTask_BT_ChangeState : public UBTTaskNode {
	GENERATED_BODY()
	
public:
	UTask_BT_ChangeState();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	E_EnemyState ChangeState;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
