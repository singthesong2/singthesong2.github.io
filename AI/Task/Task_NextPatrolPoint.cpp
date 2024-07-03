// Fill out your copyright notice in the Description page of Project Settings.

#include "Task_NextPatrolPoint.h"
#include "MyCharacterAI.h"
#include "MyCharacterAI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"

UTask_BT_NextPatrolPoint::UTask_BT_NextPatrolPoint() {
	NodeName = TEXT("Task_NextPatrolPoint");
}

EBTNodeResult::Type UTask_BT_NextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//auto const ControllingPawn = Cast<ACharacter_AI>(OwnerComp.GetAIOwner()->GetPawn());
	auto const ControllingPawn = Cast<AMyCharacterAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(ControllingPawn) == false)	{
		return EBTNodeResult::Failed;
	}
	else {
		FTransform Trans = ControllingPawn->GetNextPatrolPosition();
		FVector vec = Trans.GetLocation();
		// FQuat 회전 값을 FRotator 회전 값으로 변환
		FRotator rot = FRotator(Trans.GetRotation());
		
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s : %s"), *ControllingPawn->GetActorLocation().ToString(), *Trans.GetLocation().ToString()));
		//ControllingPawn->LookAtPoint(Trans.GetLocation());
		
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyCharacterAI_Controller::BB_NextVector, vec);
		OwnerComp.GetBlackboardComponent()->SetValueAsRotator(AMyCharacterAI_Controller::BB_NextRotator, rot);

		return EBTNodeResult::Succeeded;
	}
}
