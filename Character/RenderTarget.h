// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RenderTarget.generated.h"

UCLASS()
class SPECTER_API ARenderTarget : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneCaptureComponent2D> SceneCapture;
	
public:
	ARenderTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class ATrain_Rail> Train;

	UPROPERTY()
	TObjectPtr<class AMyCharacter> Player;

protected:
	virtual void Tick(float DeltaSeconds) override;

public:
	FORCEINLINE class USphereComponent* GetSphere() const { return Sphere; }
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE class USceneCaptureComponent2D* GetSceneCapture() const { return SceneCapture; }
};
