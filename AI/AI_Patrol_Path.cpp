// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Patrol_Path.h"

#include "Components/SplineComponent.h"

// Sets default values
AAI_Patrol_Path::AAI_Patrol_Path() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = Spline;
}

// Called when the game starts or when spawned
void AAI_Patrol_Path::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AAI_Patrol_Path::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	
}

TTuple<FVector, int32, bool> AAI_Patrol_Path::GetNextPosition(int32 Index, bool Forward) {
	int32 NextIndex = 0;
	bool NextForward = Forward;

	if (Spline->IsClosedLoop() == true)	{
		NextIndex = (Index + 1) % Spline->GetNumberOfSplinePoints();
	}
	else {
		if (Forward == true) {
			NextIndex = Index + 1;
			if (NextIndex > Spline->GetNumberOfSplinePoints() - 1) {
				NextForward = false;
				NextIndex = Spline->GetNumberOfSplinePoints() - 2;
			}
		}
		else {
			NextIndex = Index - 1;
			if (NextIndex < 0) {
				NextForward = true;
				NextIndex = 1;
			}
		}
	}

	FVector NextPosition = Spline->GetLocationAtSplinePoint(NextIndex, ESplineCoordinateSpace::World);
	return MakeTuple(NextPosition, NextIndex, NextForward);
}
