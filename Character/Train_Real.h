// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train_Real.generated.h"

UCLASS()
class SPECTER_API ATrain_Real : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UBoxComponent> Main;
	
public:
	ATrain_Real();

	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<class ATrain_Real> Main_Real_Train;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<class ATrain_Rail> Rail_Train;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<class ATrain_Between> Between_Train;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FVector TrainPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FVector TrainPositionPrev;
	
public:
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return Mesh; }
	//FORCEINLINE class UBoxComponent* GetMainBox() const { return Main; }
};
