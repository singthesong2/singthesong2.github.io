// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_FireWeapon.generated.h"

UCLASS()
class SPECTER_API UBTTask_FireWeapon : public UBTTask_BlackboardBase {
	GENERATED_BODY()

public:
	UBTTask_FireWeapon(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void Shoot(bool Newbool);

	TObjectPtr<class AMyCharacterAI_Controller> Controller;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	bool ShouldFire = true;
	
};
