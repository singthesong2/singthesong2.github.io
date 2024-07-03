// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train_Rail.generated.h"

UCLASS()
class SPECTER_API ATrain_Rail : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> TrainMesh;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UStaticMeshComponent> FrontWheel;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UStaticMeshComponent> BackWheel;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UStaticMeshComponent> FWheel1;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UStaticMeshComponent> FWheel2;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UStaticMeshComponent> BWheel1;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UStaticMeshComponent> BWheel2;

public:
	ATrain_Rail();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<class ATrain_Rail> Front_Train;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<class ATrain_Rail> Back_Train;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	// TObjectPtr<class ATrain_Real> Real_Train;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<class ATrain_Between> Train_Between;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<class ATrain_Track> Train_Track;

	FVector Location_Cur;
	FVector Location_Prv;
	FRotator Rotator_Cur;
	FRotator Rotator_Prv;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<class USplineComponent> Train_Spline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	float Train_Speed = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	float Train_Spline_Position = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float Train_MaxSpeed = 5000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	float Train_Length = 1200;

	float Train_Height = 0;
	
public:
	FORCEINLINE class USceneComponent* GetMainBox() const { return Root; }
	FORCEINLINE class UStaticMeshComponent* GetTrainMesh() const { return TrainMesh; }
	// FORCEINLINE class UStaticMeshComponent* GetFrontWheel() const { return FrontWheel; }
	// FORCEINLINE class UStaticMeshComponent* GetBackWheel() const { return BackWheel; }
	// FORCEINLINE class UStaticMeshComponent* GetFWheel1() const { return FWheel1; }
	// FORCEINLINE class UStaticMeshComponent* GetFWheel2() const { return FWheel2; }
	// FORCEINLINE class UStaticMeshComponent* GetBWheel1() const { return BWheel1; }
	// FORCEINLINE class UStaticMeshComponent* GetBWheel2() const { return BWheel2; }
};
