// Fill out your copyright notice in the Description page of Project Settings.

#include "Task_FireWeapon.h"

#include "MyCharacterAI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_FireWeapon::UBTTask_FireWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	bCreateNodeInstance = true;
	NodeName = "Fire Weapon";
}

EBTNodeResult::Type UBTTask_FireWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Controller = Cast<AMyCharacterAI_Controller>(OwnerComp.GetAIOwner());
	if (Controller == nullptr) {
		return EBTNodeResult::Failed;
	}

	Shoot(ShouldFire);
	return EBTNodeResult::Succeeded;
}

void UBTTask_FireWeapon::Shoot(bool Newbool) {
	if (Newbool == true) {
		if (Controller->Timer_GunFire.IsValid() == false) {
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("TaskFire")));
			Controller->StartWeaponFire();
		}
		return;
	}
	Controller->StopWeaponFire();
}
