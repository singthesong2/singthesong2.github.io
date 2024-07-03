// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI_Patrol_Path.generated.h"

UCLASS()
class SPECTER_API AAI_Patrol_Path : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	class USplineComponent* Spline;
	
public:	
	// Sets default values for this actor's properties
	AAI_Patrol_Path();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TTuple<FVector, int32, bool> GetNextPosition(int32 Index, bool Forward);

	FORCEINLINE class USplineComponent* GetSpline() const { return Spline; }
};
