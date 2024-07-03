// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Search_Point_Pool.h"
#include "AI_Search_Point.h"

AAI_Search_Point_Pool::AAI_Search_Point_Pool() {
	PrimaryActorTick.bCanEverTick = true;
}

void AAI_Search_Point_Pool::BeginPlay() {
	Super::BeginPlay();

	for (int32 i = 0; i < Point_Prepare; i += 1) {
		TObjectPtr<AAI_Search_Point> Point = ExtendPooled(this);
		Point_Array.Add(Point);
	}
}

void AAI_Search_Point_Pool::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

TObjectPtr<AAI_Search_Point> AAI_Search_Point_Pool::PopPooledActor(AActor* Spawner) {
	TObjectPtr<AAI_Search_Point> PooledActor = nullptr;
	if (Point_Array.Num() > 0) {
		PooledActor = Point_Array.Pop();
		//PooledActor = Point_Array[Point_Index++];
	}
	else {
		PooledActor = ExtendPooled(Spawner);
	}
	
	//PooledActor->SetActorHiddenInGame(false);
	PooledActor->AttachToActor(Spawner, FAttachmentTransformRules::KeepRelativeTransform);
	PooledActor->InPool = true;
	
	return PooledActor;
}

void AAI_Search_Point_Pool::PushPooledActor(TObjectPtr<AAI_Search_Point> ActorToReturn) {
	//ActorToReturn->SetActorHiddenInGame(true);
	ActorToReturn->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	ActorToReturn->InPool = false;
	//ActorToReturn->SetActorLocation(FVector(0, 0, 0));
	
	Point_Array.Push(ActorToReturn);
}

TObjectPtr<AAI_Search_Point> AAI_Search_Point_Pool::ExtendPooled(AActor* Spawner) {
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Ext")));

	FActorSpawnParameters Params;
	Params.Owner = Spawner;
	Params.Instigator = GetInstigator();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TObjectPtr<AAI_Search_Point> PooledActor;
	PooledActor = GetWorld()->SpawnActor<AAI_Search_Point>(Points_Class, FVector(0,0,0), FRotator(0,0,0), Params);
	PooledActor->SetPool(this);
	
	//PooledActor->SetActorHiddenInGame(true);
	PooledActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	//PooledActor->SetActorLocation(FVector(0, 0, 0));
	PooledActor->InPool = false;
	
	return PooledActor;
}
