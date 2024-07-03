// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterAI_Train.h"

#include "DrawDebugHelpers.h"
#include "Train_Between.h"
#include "Train_Helper.h"
#include "Train_Rail.h"
#include "Train_Real.h"
#include "MyCharacterAI_Squad.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ArrowComponent.h"

AMyCharacterAI_Train::AMyCharacterAI_Train() {
	PrimaryActorTick.bCanEverTick = true;

	TrainCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeshCapsule"));
	TrainCapsule->InitCapsuleSize(34.f, 88.0f);
	TrainCapsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	TrainCapsule->CanCharacterStepUpOn = ECB_No;
	TrainCapsule->SetShouldUpdatePhysicsVolume(true);
	TrainCapsule->SetCanEverAffectNavigation(false);
	TrainCapsule->bDynamicObstacle = true;
	TrainCapsule->SetupAttachment(GetCapsuleComponent());

	GetAimCamera()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GetWidgetIcon()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	//GetArrowComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	GetAimCamera()->SetupAttachment(TrainCapsule);
	GetWidgetIcon()->SetupAttachment(TrainCapsule);
	GetMesh()->SetupAttachment(TrainCapsule);
	//GetArrowComponent()->SetupAttachment(TrainCapsule);
}

void AMyCharacterAI_Train::BeginPlay() {
	Super::BeginPlay();

	// FActorSpawnParameters Params;
	// Params.Owner = this;
	// Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// Helper = GetWorld()->SpawnActor<ATrain_Helper>(GetActorLocation(), GetActorRotation(), Params);
	// Helper->HelperOwner = this;
}

void AMyCharacterAI_Train::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	/*if (IsValid(Squad) == true) {
		if (Squad->Active == false) {
			return;
		}
	}
	else {
		float Range = (UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation() - this->GetActorLocation()).Size();
		if (Range > Active_Range) {
			return;
		}
	}*/

	if (Active_Check == false) {
		return;
	}

	if (IsValid(Between) == true) {
		if (IsValid(Between->Rail_Train) == true) {
			FRotator tVecRot = Between->Rail_Train->GetActorRotation() + FRotator(0, 90, 0);
			FVector tVec = Between->Rail_Train->GetActorLocation() - tVecRot.RotateVector(Between->GetActorLocation() - GetActorLocation());
			FRotator tRot;
			tRot.Roll = GetActorRotation().Roll;
			tRot.Pitch = GetActorRotation().Pitch;
			tRot.Yaw = Between->Rail_Train->GetActorRotation().Yaw + GetActorRotation().Yaw + 90;
			TrainCapsule->SetWorldLocation(tVec, false, nullptr, ETeleportType::TeleportPhysics);
			TrainCapsule->SetWorldRotation(tRot, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}

	// if (IsValid(Between) == true) {
	// 	if (IsValid(Between->Real_Train) == true) {
	// 		FRotator tVecRot = Between->Real_Train->GetActorRotation() + FRotator(0, 90, 0);
	// 		FVector tVec = Between->Real_Train->GetActorLocation() - tVecRot.RotateVector(Between->GetActorLocation() - GetActorLocation());
	// 		FRotator tRot;
	// 		tRot.Roll = GetActorRotation().Roll;
	// 		tRot.Pitch = GetActorRotation().Pitch;
	// 		tRot.Yaw = Between->Real_Train->GetActorRotation().Yaw + GetActorRotation().Yaw + 90;
	// 		TrainCapsule->SetWorldLocation(tVec, false, nullptr, ETeleportType::TeleportPhysics);
	// 		TrainCapsule->SetWorldRotation(tRot, false, nullptr, ETeleportType::TeleportPhysics);
	// 	}
	// }

	if (bDead == true) {
		if (Flag_Dead == false) {
			Flag_Dead = true;
			//AttachToActor(Between->Real_Train, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AMyCharacterAI_Train::LookAtPoint(FVector Point, float PlayRate) {
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("TrainTurn %s"), *Point.ToString()));

	//DrawDebugPoint(GetWorld(), Point, 10, FColor::Red, false, 10);
	FVector tVector = Point;
	// if (IsValid(Between) == true) {
	// 	if (IsValid(Between->Real_Train) == true) {
	//  		tVector += -(Between->Real_Train->GetActorLocation());
	//  	}
	// }
	//DrawDebugPoint(GetWorld(), tVector, 10, FColor::Black, false, 10);

	Super::LookAtPoint(tVector, PlayRate);
}

FVector AMyCharacterAI_Train::FixTrainLocation(FVector Point) {
	FVector tVec = GetActorLocation();
	
	if (IsValid(Between) == true) {
		if (IsValid(Between->Rail_Train) == true) {
			FRotator tRot = Between->Rail_Train->GetActorRotation();
			tRot.Yaw += 90;
			tRot.Yaw *= -1;
			tVec = Between->GetActorLocation() - tRot.RotateVector(Between->Rail_Train->GetActorLocation() - Point);
			DrawDebugSphere(GetWorld(), tVec, 50, 12, FColor::Green, false, 20, 0, 5);
		}
	}
	
	// if (IsValid(Between) == true) {
	// 	if (IsValid(Between->Real_Train) == true) {
	// 		FRotator tRot = Between->Real_Train->GetActorRotation();
	// 		tRot.Yaw += 90;
	// 		tRot.Yaw *= -1;
	// 		tVec = Between->GetActorLocation() - tRot.RotateVector(Between->Real_Train->GetActorLocation() - Point);
	// 	}
	// }
	
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Fix %s"), *tVec.ToString()));
	//DrawDebugSphere(GetWorld(), tVec, 50, 12, FColor::Green, false, 5, 0, 5);
	return tVec;
}

void AMyCharacterAI_Train::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const {
	//Location = TrainCapsule->GetComponentLocation() + FVector(0,0,80);
	Location = GetActorLocation() + FVector(0,0,80); 
	//Rotation = GetMesh()->GetSocketRotation("Sight");
	Rotation.Roll = GetMesh()->GetSocketTransform("Sight", RTS_World).Rotator().Roll;
	Rotation.Pitch = GetMesh()->GetSocketTransform("Sight", RTS_World).Rotator().Pitch;
	Rotation.Yaw = GetMesh()->GetSocketTransform("Sight", RTS_World).Rotator().Yaw;
}

void AMyCharacterAI_Train::ActiveDead() {
	Super::ActiveDead();

	TrainCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
