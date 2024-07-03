// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train_Helper.generated.h"

UCLASS()
class SPECTER_API ATrain_Helper : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBillboardComponent> Billboard;

public:
	ATrain_Helper();

	UPROPERTY()
	TObjectPtr<class AMyCharacterAI_Train> HelperOwner;

	UPROPERTY()
	TObjectPtr<class AMyCharacter> Player;

	virtual void Tick(float DeltaSeconds) override;
};
