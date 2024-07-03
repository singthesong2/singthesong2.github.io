// Fill out your copyright notice in the Description page of Project Settings.

#include "Train_Rail.h"

#include "Train_Track.h"
#include "Components/SplineComponent.h"

ATrain_Rail::ATrain_Rail() {
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	TrainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainMesh"));
	TrainMesh->SetRelativeLocation(FVector(0, 0, 0));
	TrainMesh->SetupAttachment(RootComponent);

	// FrontWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontWheel"));
	// FrontWheel->SetRelativeLocation(FVector(1620, 0, -50));
	// FrontWheel->SetupAttachment(RootComponent);
	//
	// BackWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackWheel"));
	// BackWheel->SetRelativeLocation(FVector(-2170, 0, -50));
	// BackWheel->SetupAttachment(RootComponent);
	//
	// FWheel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FWheel1"));
	// FWheel1->SetRelativeLocation(FVector(85, 0, -60));
	// FWheel1->SetupAttachment(FrontWheel);
	//
	// FWheel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FWheel2"));
	// FWheel2->SetRelativeLocation(FVector(-100, 0, -60));
	// FWheel2->SetupAttachment(FrontWheel);
	//
	// BWheel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BWheel1"));
	// BWheel1->SetRelativeLocation(FVector(85, 0, -60));
	// BWheel1->SetupAttachment(BackWheel);
	//
	// BWheel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BWheel2"));
	// BWheel2->SetRelativeLocation(FVector(-100, 0, -60));
	// BWheel2->SetupAttachment(BackWheel);
}

void ATrain_Rail::BeginPlay() {
	Super::BeginPlay();
	if (IsValid(Front_Train) == true) {
		AddTickPrerequisiteActor(Front_Train);
	}
	
	if (IsValid(Train_Track) == true) {
		Train_Spline = Train_Track->GetSpline();
		Train_Height = GetActorLocation().Z - Train_Track->GetActorLocation().Z;
	}
	
	if (IsValid(Train_Spline) == true) {
		Train_Spline_Position = Train_Spline->GetDistanceAlongSplineAtSplineInputKey(Train_Spline->FindInputKeyClosestToWorldLocation(GetActorLocation()));
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("%s %f"), *this->GetName(), Train_Spline_Position));
	}
}

void ATrain_Rail::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	Location_Prv = Location_Cur;
	Location_Cur = GetActorLocation();
	Rotator_Prv = Rotator_Cur;
	Rotator_Cur = GetActorRotation();
	
	if (IsValid(Train_Spline) == true) {
		Train_Spline_Position += Train_Speed * DeltaSeconds;
		
		if (Train_Speed < Train_MaxSpeed)	{ Train_Speed += 50; }
		else								{ Train_Speed = Train_MaxSpeed; }
		
		if (Train_Spline_Position + Train_Length > Train_Spline->GetSplineLength()) {
			Train_Spline_Position = Train_Length;
		}
		
		//FVector Train_FLocation = Train_Spline->GetLocationAtDistanceAlongSpline(Train_Spline_Position + Train_Length, ESplineCoordinateSpace::World);
		//FVector Train_BLocation = Train_Spline->GetLocationAtDistanceAlongSpline(Train_Spline_Position - Train_Length, ESplineCoordinateSpace::World);
		//SetActorLocation((Train_FLocation + Train_BLocation) / 2, false, nullptr, ETeleportType::None);
		//SetActorRotation((Train_FLocation - Train_BLocation).Rotation(), ETeleportType::None);

		FVector Train_SplineLocation = Train_Spline->GetLocationAtDistanceAlongSpline(Train_Spline_Position, ESplineCoordinateSpace::World);
		FVector Train_NormalVector;

		if ((IsValid(Front_Train) == false) || (IsValid(Back_Train) == false)) {
			Train_NormalVector = Train_Spline->GetDirectionAtDistanceAlongSpline(Train_Spline_Position, ESplineCoordinateSpace::World);
		}
		else {
			Train_NormalVector = Front_Train->GetActorLocation() - Back_Train->GetActorLocation();
		}

		SetActorLocation(Train_SplineLocation + FVector(0, 0, Train_Height), false, nullptr, ETeleportType::None);
		SetActorRotation(Train_NormalVector.Rotation(), ETeleportType::ResetPhysics);
		
		// GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Yellow, FString::Printf(TEXT("%s %f"), *this->GetName(), Train_Spline_Position));
		// DrawDebugPoint(GetWorld(), Train_FLocation, 5, FColor::Red, false, 0.1, 0);
		// DrawDebugPoint(GetWorld(), Train_BLocation, 5, FColor::Blue, false, 0.1, 0);
	}
}
