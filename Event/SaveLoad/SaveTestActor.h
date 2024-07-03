// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../EventActor.h"
#include "SaveInterface.h"
#include "SaveTestActor.generated.h"

UCLASS()
class SPECTER_API ASaveTestActor : public AEventActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere);
	UStaticMeshComponent* Mesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//virtual void SetActorToSavable(const UObject* WorldContextObject = nullptr) override;
	virtual void SetActorToSavable() override;
	UPROPERTY(EditAnywhere)
		FColor Color;
	virtual void OnInteractRangeEntered_Implementation(class AMyCharacter* Target) override;
	virtual void OnInteractRangeExited_Implementation(class AMyCharacter* Target) override;
};
