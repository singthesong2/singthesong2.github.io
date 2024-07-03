// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Search_Point_Manager.h"
#include "AI_Search_Point_Pool.h"
#include "AI_Search_Point.h"
#include "NavigationSystem.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAI_Search_Point_Manager::AAI_Search_Point_Manager() {
	PrimaryActorTick.bCanEverTick = true;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	RootComponent = Arrow;
	Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Arrow->SetRelativeRotation(FRotator(90, 0, 0));
	Arrow->SetVisibility(true);
	Arrow->SetHiddenInGame(true);

	Arrow->ArrowColor = FColor(0, 0, 0);
	Arrow->ArrowSize = 2;
}

// Called when the game starts or when spawned
void AAI_Search_Point_Manager::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Points_Pool_Class, FoundActors);
	for (int32 i = 0; i < FoundActors.Num(); i += 1) {
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("ASPM %s"), *FoundActors[i]->GetName()));
		Points_Pool = Cast<AAI_Search_Point_Pool>(FoundActors[i]);
	}
	NavSystem = UNavigationSystemV1::GetNavigationSystem(this->GetWorld());
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("manager instig %s"), *GetInstigator()->GetName()));
}

void AAI_Search_Point_Manager::Destroyed() {
	Super::Destroyed();
	ClearSearchPoint();
}

void AAI_Search_Point_Manager::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void AAI_Search_Point_Manager::ClearSearchPoint() {
	for (int32 i = 0; i < Points.Num(); i += 1) {
		if (IsValid(Points[i]) == true) {
			Points[i]->ReleasePool();
			//Points[i]->Destroy();
		}
	}

	Points.Empty();
}

void AAI_Search_Point_Manager::GenerateSearchPoint(int32 Size, int32 Between) {
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%d GSP"), this->GetUniqueID()));

	for (int32 x = -Size; x <= Size; x += 1) {
		for (int32 y = -Size; y <= Size; y += 1) {
			FVector vec = FVector((float)(x) * Between, (float)(y) * Between, 0.0f);
			FNavLocation out;
			// AAI_Search_Point* pt = Points_Pool->PopPooledActor(this);
			// pt->SetActorLocation(GetActorLocation() + vec);
			// Points.Add(pt);
			
			if (NavSystem->ProjectPointToNavigation(GetActorLocation() - vec, out, FVector(500, 500, 500)) == true) {
				AAI_Search_Point* pt = Points_Pool->PopPooledActor(this);
				pt->SetActorLocation(out);
				Points.Add(pt);
			}
		}
	}
}

bool AAI_Search_Point_Manager::PointsLeft() {
	if (Points.Num() > 0) {
		return true;
	}
	return false;
}

void AAI_Search_Point_Manager::ReleasePoint(AAI_Search_Point* Point) {
	for (int32 i = Points.Num() - 1; i >= 0; i += -1) {
		if ((IsValid(Points[i]) == true) && (IsValid(Point) == true)) {
			if (Points[i] == Point) {
				Points.RemoveAt(i);
			}
		}
	}
}
