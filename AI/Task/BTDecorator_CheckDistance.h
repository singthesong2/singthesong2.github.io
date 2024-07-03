﻿#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CheckDistance.generated.h"

UCLASS()
class SPECTER_API UBTDecorator_CheckDistance : public UBTDecorator_BlackboardBase {
	GENERATED_BODY()

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UBTDecorator_CheckDistance(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, Category = Condition)
	float Distance = 1000.f;
};
