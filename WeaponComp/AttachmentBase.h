// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttachmentBase.generated.h"

UCLASS()
class SPECTER_API AAttachmentBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttachmentBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Init_OnCreate();

public:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* DefaultSceneComponent;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* AttachmentMesh;

};
