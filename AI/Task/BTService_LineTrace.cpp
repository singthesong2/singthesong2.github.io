// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_LineTrace.h"
#include "AIController.h"
#include "MyCharacterAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameFramework/Character.h"

UBTService_LineTrace::UBTService_LineTrace() {
	NodeName = "Service LineTrace";

	//BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
	
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

void UBTService_LineTrace::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const {

}

void UBTService_LineTrace::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AIChar = Cast<AMyCharacterAI>(OwnerComp.GetAIOwner()->GetPawn());
	Blackboard = OwnerComp.GetBlackboardComponent();
}

void UBTService_LineTrace::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	
}

void UBTService_LineTrace::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	if (IsValid(AIChar) == true) {
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));
		if (IsValid(TargetActor) == true) {
			FVector Start = AIChar->GetMesh()->GetSocketLocation("Sight");
			FHitResult Hit;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(OwnerComp.GetAIOwner()->GetPawn());
			
			GetWorld()->LineTraceSingleByChannel(Hit, Start, TargetActor->GetActorLocation(), ECC_GameTraceChannel2, CollisionParams);
			
			if (Hit.GetActor() != TargetActor) {
				
			}
		}
	}
}
