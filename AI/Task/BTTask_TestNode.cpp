// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TestNode.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TestNode::UBTTask_TestNode() {
	NodeName = TEXT("TestNode");
}

EBTNodeResult::Type UBTTask_TestNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	GEngine->AddOnScreenDebugMessage(-1, 1000000000, FColor::Yellow, FString::Printf(TEXT("TestNode")));
	
	return EBTNodeResult::Succeeded;
}
