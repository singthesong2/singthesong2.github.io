// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterBase.h"

#include "AIEnum.h"
#include "MyCharacterAI_Controller.h"
#include "Characte_Ani.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AMyCharacterBase::AMyCharacterBase() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacterBase::BeginPlay() {
	Super::BeginPlay();

	HP = HP_Max;
}

// Called every frame
void AMyCharacterBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AMyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AMyCharacterBase::GetDead() {
	return bDead;
}

void AMyCharacterBase::GetDamage(float Damage) {
	HP += -Damage;
	HP = FMath::Clamp(HP, 0, HP_Max);
	//GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Green, FString::Printf(TEXT("%s HP %.1f/%.1f"), *this->GetName(), HP, HP_Max));
	if (HP <= 0) {
		ActiveDead();
	}
}

void AMyCharacterBase::ActiveDead() {
	bDead = true;
	if (GetController() != nullptr) {
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		AMyCharacterAI_Controller* Ctrl = Cast<AMyCharacterAI_Controller>(GetController());
		if (Ctrl != nullptr) {
			Ctrl->StateChange(E_EnemyState::E_StateDead);
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Dead")));
}

bool AMyCharacterBase::IsDead() {
	auto AnimInstance = Cast<UCharacte_Ani>(GetMesh()->GetAnimInstance());

	if (nullptr == AnimInstance)
		return false;

	AnimInstance->PlayMontage(FName("Death"));
	return true;
}

FVector AMyCharacterBase::FixTrainLocation(FVector Point) {
	return Point;
}

bool AMyCharacterBase::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const {
	static const FName NAME_AILineOfSight = FName(TEXT("TestPawnLineOfSight"));
	FHitResult HitResult;

	FVector SightTargetLocation = GetMesh()->GetSocketLocation("neck_01");
	// bool bRayHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, SightTargetLocation
	// 		, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_GameTraceChannel3))
	// 		, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));
	
	bool bRayHit = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, SightTargetLocation, ECC_GameTraceChannel3, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));	
	if (bRayHit == false || (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsOwnedBy(this))) {
		OutSeenLocation = SightTargetLocation;
		OutSightStrength = 1;
		return true;
	}
	OutSightStrength = 0;
	return false;
}