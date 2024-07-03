// Fill out your copyright notice in the Description page of Project Settings.

#include "Task_CheckCover.h"
#include "MyCharacterAI.h"
#include "MyCharacterAI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"

UTask_BT_CheckCover::UTask_BT_CheckCover() {
	NodeName = TEXT("Task_CheckCover");
}

EBTNodeResult::Type UTask_BT_CheckCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//auto const ControllingPawn = Cast<ACharacter_AI>(OwnerComp.GetAIOwner()->GetPawn());
	auto const ControllingPawn = Cast<AMyCharacterAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(ControllingPawn) == true) {
		ControllingPawn->CheckCover();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
