// Fill out your copyright notice in the Description page of Project Settings.

#include "Train_Between.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

ATrain_Between::ATrain_Between() {
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = Mesh;

	// Main = CreateDefaultSubobject<UBoxComponent>(TEXT("MainBox"));
	// Main->SetRelativeLocation(FVector(0, 0, 400));
	// Main->SetBoxExtent(FVector(1750, 600, 600));
	// Main->SetupAttachment(RootComponent);
	//
	// Back = CreateDefaultSubobject<UBoxComponent>(TEXT("BackBox"));
	// Back->SetRelativeLocation(FVector(-1700, 0, 400));
	// Back->SetBoxExtent(FVector(50, 600, 600));
	// Back->SetupAttachment(RootComponent);
	//
	// Front = CreateDefaultSubobject<UBoxComponent>(TEXT("FrontBox"));
	// Front->SetRelativeLocation(FVector(1700, 0, 400));
	// Front->SetBoxExtent(FVector(50, 600, 600));
	// Front->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetRelativeLocation(FVector(0, 0, 1000));
	Arrow->SetRelativeRotation(FRotator(0, 0, 0));
	Arrow->ArrowSize = 10;
	Arrow->SetupAttachment(RootComponent);
}
