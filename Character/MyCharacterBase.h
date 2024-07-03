// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "MyCharacterBase.generated.h"

UCLASS()
class SPECTER_API AMyCharacterBase : public ACharacter, public IAISightTargetInterface {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacterBase();

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;

	UPROPERTY(BlueprintReadOnly)
	bool bJog = false;

	UPROPERTY(BlueprintReadOnly)
	bool bAim = false;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterState")
	float HP_Max = 100.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState")
	float HP = HP_Max;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState")
	bool TrainMap = false;
	
	bool bDeathFound = false;
	bool bHideInBush = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool GetDead();

	UFUNCTION(BlueprintCallable)
	virtual void GetDamage(float Damage);

	virtual void ActiveDead();

	UFUNCTION(BlueprintCallable)
	bool IsDead();

	UFUNCTION(BlueprintPure)
	virtual FVector FixTrainLocation(FVector Point);

	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const override;
};
