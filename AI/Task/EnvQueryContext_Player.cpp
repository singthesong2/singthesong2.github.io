#include "EnvQueryContext_Player.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "AISystem.h"
#include "VisualLogger/VisualLogger.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EnvQueryContext_Player)

UEnvQueryContext_Player::UEnvQueryContext_Player(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
}

void UEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const {
	AActor* QueryOwner = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UE_CVLOG(GET_AI_CONFIG_VAR(bAllowControllersAsEQSQuerier) == false && Cast<AController>(QueryOwner) != nullptr, QueryOwner, LogEQS, Warning, TEXT("Using Controller as query's owner is dangerous since Controller's location is usually not what you expect it to be!"));
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, QueryOwner);
}

