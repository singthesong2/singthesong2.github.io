// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetNextPoint.h"
#include "MyCharacterAI.h"
#include "MyCharacterAI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_SetNextPoint::UBTTask_SetNextPoint() {
	NodeName = TEXT("UBTTask_SetNextPoint");

	Point.SelectedKeyName = FName("Point");
	Point.SelectedKeyType = UBlackboardKeyType_Vector::StaticClass(); // Set the data type to Vector
}

EBTNodeResult::Type UBTTask_SetNextPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	FVector vec;
	AActor* Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Point.SelectedKeyName));
	if (Actor) {
		vec = Actor->GetActorLocation();
	}
	else {
		vec = OwnerComp.GetBlackboardComponent()->GetValueAsVector(Point.SelectedKeyName);
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyCharacterAI_Controller::BB_NextVector, vec);

	return EBTNodeResult::Succeeded;
}
