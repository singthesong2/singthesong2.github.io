// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_LineTrace.generated.h"

UCLASS()
class SPECTER_API UBTService_LineTrace : public UBTService {
	GENERATED_BODY()

	bool bFuncCall = false;
public:
	UBTService_LineTrace();

	UPROPERTY()
	TObjectPtr<class AMyCharacterAI> AIChar;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> Blackboard;

	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector BlackboardKey;
	
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
