// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_CheckSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AIController.h"

UBTDecorator_CheckSpeed::UBTDecorator_CheckSpeed(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	NodeName = "Check Speed";
	bCreateNodeInstance = true;

	BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
}

bool UBTDecorator_CheckSpeed::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* Controller = OwnerComp.GetAIOwner();
	bool OutsideRange = false;

	if ((Controller == nullptr) || (BB == nullptr)) {
		return OutsideRange;
	}
	UBTDecorator_CheckSpeed::StaticClass();
	auto MyID = BB->GetKeyID(BlackboardKey.SelectedKeyName);

	UObject* KeyValue = BB->GetValue<UBlackboardKeyType_Object>(MyID);
	AActor* TargetActor = Cast<AActor>(KeyValue);
	if (TargetActor == nullptr) {
		return OutsideRange;
	}

	if (Controller->GetPawn()->GetDistanceTo(TargetActor) >= Distance) {
		OutsideRange = true;
	}

	return OutsideRange;
}
