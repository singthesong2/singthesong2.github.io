// Fill out your copyright notice in the Description page of Project Settings.

#include "Task_TurnPoint.h"
#include "AIController.h"
#include "MyCharacterAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UTask_BT_TurnPoint::UTask_BT_TurnPoint() {
	NodeName = TEXT("Task_TurnPoint");

	Point.SelectedKeyName = FName("Point");
	Point.SelectedKeyType = UBlackboardKeyType_Vector::StaticClass(); // Set the data type to Vector

	PlayRate = 1.0f;
	//PlayRate.SelectedKeyName = FName("PlayRate");
	//PlayRate.SelectedKeyType = UBlackboardKeyType_Float::StaticClass(); // Set the data type to Vector
}

EBTNodeResult::Type UTask_BT_TurnPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//auto const ControllingPawn = Cast<ACharacter_AI>(OwnerComp.GetAIOwner()->GetPawn());
	auto const ControllingPawn = Cast<AMyCharacterAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(ControllingPawn) == true)	{
		FVector Vector;
		AActor* Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Point.SelectedKeyName));
		if (Actor) {
			Vector = Actor->GetActorLocation();
		}
		else {
			Vector = OwnerComp.GetBlackboardComponent()->GetValueAsVector(Point.SelectedKeyName);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("%s"), *Vector.ToString()));

		//FTransform Trans = ControllingPawn->GetNextPatrolPosition();
		ControllingPawn->LookAtPoint(Vector, PlayRate);
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}