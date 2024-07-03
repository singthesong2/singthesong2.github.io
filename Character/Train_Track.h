// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train_Track.generated.h"

UCLASS()
class SPECTER_API ATrain_Track : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USplineComponent> Spline;
	
public:	
	// Sets default values for this actor's properties
	ATrain_Track();

	FORCEINLINE TObjectPtr<class USplineComponent> GetSpline() const { return Spline; }
};
