// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/AnimationExecuter.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAnimationExecuter::AAnimationExecuter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAnimationExecuter::BeginPlay()
{
	Super::BeginPlay();
	if (nullptr == TargetActor)
	{
		//UE_LOG(LogTemp, Log, TEXT("Invalid target actor in %s"), *GetActorLabel());
		return;
	}
	USkeletalMeshComponent* MeshComp = TargetActor->FindComponentByClass<USkeletalMeshComponent>();
	if (nullptr == MeshComp)
	{
		//UE_LOG(LogTemp, Log, TEXT("Failed to find target actor's skeletal mesh component in %s"), *TargetActor->GetActorLabel());
		return;
	}
	TargetAnimInstance = MeshComp->GetAnimInstance();
	if (nullptr == TargetAnimInstance)
	{
		//UE_LOG(LogTemp, Log, TEXT("Failed to find UAnimInstance component from target actor in %s"), *GetActorLabel());
		return;
	}
}

// Called every frame
void AAnimationExecuter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAnimationExecuter::ExecuteEvent_Implementation()
{
	if (nullptr == AnimeAsset) 
	{
		//UE_LOG(LogTemp, Log, TEXT("Invalid animation asset in %s"), *GetActorLabel());
	}
	if (nullptr == TargetActor)
	{
		//UE_LOG(LogTemp, Log, TEXT("Invalid target actor in %s"), *GetActorLabel());
		return;
	}
	//If duration require calculate like this
	//const float Duration = PlayAnimMontage(AnimMontage, InPlayRate) / (InPlayRate * AnimMontage->RateScale) 
	TargetAnimInstance->Montage_Play(AnimeMontag, 1.0f);
	UE_LOG(LogTemp, Log, TEXT("Play animation %s"), *AnimeMontag->GetName());
}