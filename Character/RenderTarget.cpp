// Fill out your copyright notice in the Description page of Project Settings.

#include "RenderTarget.h"
#include "MyCharacter.h"
#include "Train_Rail.h"
#include "Train_Real.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ARenderTarget::ARenderTarget() {
	PrimaryActorTick.bCanEverTick = false;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	SpringArm->TargetArmLength = 600;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetupAttachment(RootComponent);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetRelativeLocation(FVector(0, 80, 80));
	SceneCapture->SetupAttachment(SpringArm);
}

void ARenderTarget::Tick(float DeltaSeconds) {
	// TObjectPtr<APlayerController> ctrl = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// if (IsValid(ctrl) == true) {
	// 	FVector vec = Train->GetActorLocation() - (Train->Real_Train->GetActorLocation() - ctrl->GetPawn()->GetActorLocation());
	// 	//FVector vec = Train->GetActorLocation() - ctrl->GetPawn()->GetActorLocation();
	// 	SetActorLocation(vec, false, nullptr, ETeleportType::None);
	// 	SetActorRotation(ctrl->GetControlRotation(), ETeleportType::None);
	//
	// 	if (IsValid(Player) == false) {
	// 		Player = Cast<AMyCharacter>(ctrl->GetPawn());
	// 	}
	// 	else {
	// 		SceneCapture->SetRelativeLocation(Player->GetLeftCamera()->GetRelativeLocation());
	// 		SpringArm->TargetArmLength = Player->GetCameraBoom()->TargetArmLength;
	// 	}
	// }
}
