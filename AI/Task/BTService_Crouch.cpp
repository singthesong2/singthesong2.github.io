// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/BTService_Crouch.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTService_DoCrouch::UBTService_DoCrouch() {
	NodeName = "Do Crouch";

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

void UBTService_DoCrouch::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const {

}

void UBTService_DoCrouch::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	if (IsValid(OwnerComp.GetAIOwner()) == true) {
		if (IsValid(OwnerComp.GetAIOwner()->GetCharacter()) == true) {
			OwnerComp.GetAIOwner()->GetCharacter()->Crouch();
		}
	}
}

void UBTService_DoCrouch::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	if (IsValid(OwnerComp.GetAIOwner()) == true) {
		if (IsValid(OwnerComp.GetAIOwner()->GetCharacter()) == true) {
			OwnerComp.GetAIOwner()->GetCharacter()->UnCrouch();
		}
	}
}

void UBTService_DoCrouch::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
