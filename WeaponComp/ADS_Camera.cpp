// Fill out your copyright notice in the Description page of Project Settings.


#include "ADS_Camera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AADS_Camera::AADS_Camera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(DefaultSceneComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(DefaultSceneComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AADS_Camera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AADS_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AADS_Camera::SettingCameraLag()
{
	SpringArm->bEnableCameraLag = EnableCameraLag;
	SpringArm->bEnableCameraRotationLag = EnableCameraRotationLag;
	SpringArm->CameraLagSpeed = CameraLagSpeed;
	SpringArm->CameraRotationLagSpeed = CameraRotationLagSpeed;
	SpringArm->CameraLagMaxDistance = CameraLagMaxDistance;
}

void AADS_Camera::SettingCameraFOV(float Fov_Value)
{
	Camera->SetFieldOfView(Fov_Value);
}
