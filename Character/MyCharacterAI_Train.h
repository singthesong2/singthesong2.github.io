// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacterAI.h"
#include "MyCharacterAI_Train.generated.h"

UCLASS()
class SPECTER_API AMyCharacterAI_Train : public AMyCharacterAI {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> TrainCapsule;
	
public:
	AMyCharacterAI_Train();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<class ATrain_Between> Between;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	TObjectPtr<class ATrain_Helper> Helper;

	bool Flag_Dead = false;
	
	virtual void LookAtPoint(FVector Point, float PlayRate) override;
	
	virtual FVector FixTrainLocation(FVector Point) override;

	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	virtual void ActiveDead() override;

public:
	FORCEINLINE TObjectPtr<class UCapsuleComponent> GetCapsuleTrain() const { return TrainCapsule; }
};
