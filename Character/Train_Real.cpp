// Fill out your copyright notice in the Description page of Project Settings.

#include "Train_Real.h"
#include "Train_Rail.h"

ATrain_Real::ATrain_Real() {
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = Mesh;

	// Main = CreateDefaultSubobject<UBoxComponent>(TEXT("MainBox"));
	// Main->SetRelativeLocation(FVector(0, 0, 400));
	// Main->SetBoxExtent(FVector(1700, 500, 500));
	// Main->SetupAttachment(RootComponent);
}

void ATrain_Real::Tick(float DeltaSeconds) {
	if (IsValid(Main_Real_Train) == true) {
		if (Main_Real_Train == this) {
			FTransform trans = FTransform(Main_Real_Train->Rail_Train->GetActorRotation(), GetActorLocation());
			SetActorTransform(trans, false, nullptr, ETeleportType::None);
		}
		else {
			FTransform trans = FTransform(Rail_Train->GetActorRotation(), Main_Real_Train->GetActorLocation()
				- (Main_Real_Train->Rail_Train->GetActorLocation() - Rail_Train->GetActorLocation()));
			SetActorTransform(trans, false, nullptr, ETeleportType::None);
		}
	}
}
