// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Bush.h"
#include "MyCharacterAI.h"
#include "Components/BoxComponent.h"

AAI_Bush::AAI_Bush() {
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Bush"));
	BoxCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BoxCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	BoxCollision->SetCollisionProfileName(TEXT("Bush"));
}

void AAI_Bush::BeginPlay() {
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bush::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AAI_Bush::OnOverlapEnd);

	BeginLocation = GetActorLocation();
}

void AAI_Bush::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	TObjectPtr<AMyCharacterAI> tCharacter = Cast<AMyCharacterAI>(OtherActor);
	if (IsValid(tCharacter) == true) {
		BoxCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
	}
}

void AAI_Bush::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	TObjectPtr<AMyCharacterAI> tCharacter = Cast<AMyCharacterAI>(OtherActor);
	if (IsValid(tCharacter) == true) {
		BoxCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
	}
}