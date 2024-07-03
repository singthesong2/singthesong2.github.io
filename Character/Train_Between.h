// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train_Between.generated.h"

UCLASS()
class SPECTER_API ATrain_Between : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UBoxComponent> Main;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UBoxComponent> Back;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UBoxComponent> Front;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UArrowComponent> Arrow;

public:
	ATrain_Between();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<class ATrain_Rail> Rail_Train;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	// TObjectPtr<class ATrain_Real> Real_Train;

public:
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return Mesh; }
	// FORCEINLINE class UBoxComponent* GetMainBox() const { return Main; }
	// FORCEINLINE class UBoxComponent* GetBackBox() const { return Back; }
	// FORCEINLINE class UBoxComponent* GetFrontBox() const { return Front; }
	FORCEINLINE class UArrowComponent* GetArrow() const { return Arrow; }
};
