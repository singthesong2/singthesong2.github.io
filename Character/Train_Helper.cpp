// Fill out your copyright notice in the Description page of Project Settings.

#include "Train_Helper.h"

#include "MyCharacter.h"
#include "MyCharacterAI_Train.h"
#include "Train_Between.h"
#include "Train_Rail.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"

ATrain_Helper::ATrain_Helper() {
	PrimaryActorTick.bCanEverTick = true;
	
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillBoard"));
}

void ATrain_Helper::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (HelperOwner == nullptr) {
		return;
	}
	
	if (Player == nullptr) {
		Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	}

	FVector tVec = Player->GetActorLocation();

	if (IsValid(HelperOwner->Between) == true) {
		if (IsValid(HelperOwner->Between->Rail_Train) == true) {
			FRotator tRot = HelperOwner->Between->Rail_Train->GetActorRotation();
			tRot.Yaw += 90;
			tRot.Yaw *= -1;
			tVec = HelperOwner->Between->GetActorLocation() - tRot.RotateVector(HelperOwner->Between->Rail_Train->GetActorLocation() - Player->GetActorLocation());
		}
	}
	
	SetActorLocation(tVec);
}
