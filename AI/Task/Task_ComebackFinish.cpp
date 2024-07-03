// Fill out your copyright notice in the Description page of Project Settings.

#include "Task_ComebackFinish.h"
#include "MyCharacterAI.h"
#include "MyCharacterAI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"

UTask_BT_ComebackFinish::UTask_BT_ComebackFinish() {
	NodeName = TEXT("Task_ComebackFinish");
}

EBTNodeResult::Type UTask_BT_ComebackFinish::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//auto const ControllingPawn = Cast<ACharacter_AI>(OwnerComp.GetAIOwner()->GetPawn());
	auto const ControllingPawn = Cast<AMyCharacterAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(ControllingPawn) == false)	{
		return EBTNodeResult::Failed;
	}
	else {
		AMyCharacterAI_Controller* Ctrl = Cast<AMyCharacterAI_Controller>(ControllingPawn->GetController());
		Ctrl->StateChange(E_EnemyState::E_StateOff);
		return EBTNodeResult::Succeeded;
	}
}
