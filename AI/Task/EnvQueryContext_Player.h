#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_Player.generated.h"

struct FEnvQueryContextData;
struct FEnvQueryInstance;

UCLASS()
class SPECTER_API UEnvQueryContext_Player : public UEnvQueryContext {
	GENERATED_UCLASS_BODY()
 
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
