// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_LineTrace.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Camera/CameraComponent.h"
#include "MyCharacter.h"
#include "MyCharacterAI.h"
#include "WeaponComponent.h"


UBTDecorator_LineTrace::UBTDecorator_LineTrace(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	NodeName = "Decorator LineTrace";
	bCreateNodeInstance = true;
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;

	TickInterval = 1.0f;
	TickIntervalLast = 0.0f;
	
	//BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
}

void UBTDecorator_LineTrace::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AIChar = Cast<AMyCharacterAI>(OwnerComp.GetAIOwner()->GetPawn());
	Blackboard = OwnerComp.GetBlackboardComponent();
	
}

void UBTDecorator_LineTrace::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TickIntervalLast += DeltaSeconds;
	if (TickIntervalLast < TickInterval) {
		return;
	}
	TickIntervalLast = 0.0f;
	
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Tick")));
	if (IsValid(AIChar) == true) {
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));
		if (IsValid(TargetActor) == true) {
			FVector Start = AIChar->GetMesh()->GetSocketLocation("Sight");
			FHitResult Hit;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(OwnerComp.GetAIOwner()->GetPawn());
			
			GetWorld()->LineTraceSingleByChannel(Hit, Start, TargetActor->GetActorLocation(), ECC_GameTraceChannel2, CollisionParams);
			
			if (Hit.GetActor() != TargetActor) {
				OwnerComp.RequestExecution(this);
			}
		}
	}
}

bool UBTDecorator_LineTrace::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Calcu")));
	return true;
}
