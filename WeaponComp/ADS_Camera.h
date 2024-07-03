// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADS_Camera.generated.h"

UCLASS()
class SPECTER_API AADS_Camera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AADS_Camera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USceneComponent* DefaultSceneComponent;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

public:
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool EnableCameraLag;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool EnableCameraRotationLag;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float CameraLagSpeed;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float CameraRotationLagSpeed;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float CameraLagMaxDistance;

public:
	UFUNCTION()
	void SettingCameraLag();

	UFUNCTION()
	void SettingCameraFOV(float Fov_Value);

	class UCameraComponent* GetCamera() { return Camera; }
};
