// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Floor.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	BoxCollision->SetNotifyRigidBodyCollision(true);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnOverlapBegin);
}

void AFloor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);

	if (IsValid(Player))
	{
		if (Player->MoveCharacter)
			return;

		if (!Player->MoveCharacter)
		{
			Player->MoveCharacter = true;

			Player->SetActorFlag(EndFlag);
		}
	}
}

