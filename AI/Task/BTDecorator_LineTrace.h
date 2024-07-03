#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_LineTrace.generated.h"

UCLASS()
class SPECTER_API UBTDecorator_LineTrace : public UBTDecorator {
	GENERATED_BODY()

	

protected:
	UBTDecorator_LineTrace(const FObjectInitializer& ObjectInitializer);

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	UPROPERTY()
	TObjectPtr<class AMyCharacterAI> AIChar;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> Blackboard;
	
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector BlackboardKey;

	UPROPERTY(EditAnywhere, Category = "Decorator")
	float TickInterval = 1.0f;
	
	float TickIntervalLast;
};
