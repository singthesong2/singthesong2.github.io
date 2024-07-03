// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_GetCover.h"

#include "AICoverPoint.h"
#include "MyCharacterAI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_GetCover::UBTTask_GetCover() {
	NodeName = TEXT("Task_GetCover");

	Point.SelectedKeyName = FName("Point");
	Point.SelectedKeyType = UBlackboardKeyType_Vector::StaticClass(); // Set the data type to Vector
}

EBTNodeResult::Type UBTTask_GetCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	UBlackboardComponent* tBlackboard = OwnerComp.GetBlackboardComponent();
	
	auto const tController = Cast<AMyCharacterAI_Controller>(OwnerComp.GetAIOwner());
	if (IsValid(tController) == true) {
		FCover tCover;
		if (tController->CoverManager->PickCover(tCover, tController, 1000.0f) == true) {
			tController->CoverPoint = tCover;

			//FBlackboard::FKey KeyID = tBlackboard->GetKeyID(Point.SelectedKeyName);
			tBlackboard->SetValueAsVector(Point.SelectedKeyName, tCover.Data.Location);

			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Succeeded;
}