// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExecutionInterface.h"
#include "AnimationExecuter.generated.h"

UCLASS()
class SPECTER_API AAnimationExecuter : public AActor, public IExecutionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimationExecuter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	AActor* TargetActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* AnimeAsset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AnimeMontag;
	UPROPERTY(EditAnywhere, Category = "Animation")
	FName SlotName;
	UAnimInstance* TargetAnimInstance;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ExecuteEvent_Implementation();
};
