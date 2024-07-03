// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_ChangeState.h"
#include "AIController.h"
#include "MyCharacterAI.h"
#include "MyCharacterAI_Controller.h"

UTask_BT_ChangeState::UTask_BT_ChangeState() {
	NodeName = TEXT("Task_ChangeState");
	
	ChangeState = E_EnemyState::E_StatePatrol;
}

EBTNodeResult::Type UTask_BT_ChangeState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	//auto const ControllingPawn = Cast<ACharacter_AI>(OwnerComp.GetAIOwner()->GetPawn());
	auto const ControllingPawn = Cast<AMyCharacterAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(ControllingPawn) == true)	{
		auto const AI_Ctrl = Cast<AMyCharacterAI_Controller>(ControllingPawn->GetController());

		if (IsValid(ControllingPawn) == true) {
			switch (ChangeState) {
			case E_EnemyState::E_StatePatrol:
				AI_Ctrl->StartPatrol();
				break;
			case E_EnemyState::E_StateAlert:
				AI_Ctrl->StartAlert(ControllingPawn->GetActorLocation());
				break;
			case E_EnemyState::E_StateSearch:
				AI_Ctrl->StartSearch();
				break;
			case E_EnemyState::E_StateAttack:
				AI_Ctrl->StartAttack();
				break;
			default:
				break;
			}
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
