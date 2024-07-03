// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Search_Point.h"
#include "AI_Search_Point_Manager.h"
#include "AI_Search_Point_Pool.h"
#include "MyCharacterAI.h"
#include "MyCharacterAI_Controller.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
AAI_Search_Point::AAI_Search_Point() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	RootComponent = Arrow;
	Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Arrow->SetRelativeRotation(FRotator(0, 0, 0));
	Arrow->SetVisibility(true);
	Arrow->SetHiddenInGame(true);
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Sphere->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	Sphere->SetSphereRadius(300);

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	//StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
}

// Called when the game starts or when spawned
void AAI_Search_Point::BeginPlay() {
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAI_Search_Point::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAI_Search_Point::OnOverlapEnd);
	//StimuliSource->O
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
	//	FString::Printf(TEXT("point instig %s"), *GetInstigator()->GetName()));
}

// Called every frame
void AAI_Search_Point::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	// if (CheckPoint() == true) {
	// 	ReleasePool();
	// 	Destroy();
	// }
}

// bool AAI_Search_Point::FinishPoint() {
// 	if (FVector::Distance(GetInstigator()->GetActorLocation(), GetActorLocation()) < 300) {
// 		return true;
// 	}
// 	
// 	FVector Start = GetActorLocation();
// 	FVector End = GetInstigator()->GetActorLocation() + GetInstigator()->GetActorForwardVector() * 100;
//
// 	FHitResult Hit;
// 	FCollisionQueryParams Params;
// 	Params.AddIgnoredActor(this);
// 	TArray<AActor*> ActorsToIgnore;
// 	ActorsToIgnore.Add(GetOwner());
//
// 	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End,
// 	UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore,
// 	EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, GetWorld()->GetDeltaSeconds());
//
// 	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("bool %d"), bResult));
// 	
// 	if (bResult == false) {
// 		if (abs((Start - End).Rotation().Yaw - GetInstigator()->GetActorRotation().Yaw) < 60) {
// 			if ((Start - End).Length() < 500) {
// 				return true;
// 			}
// 		}
// 	}
// 	
// 	return false;
// }
//
// bool AAI_Search_Point::CheckPoint() {
// 	if (IsValid(Parent) == true) {
// 		for (INT32 i = 0; i < Parent->AI_Array.Num(); i += 1) {
// 			AMyCharacterAI_Controller* Ctrl = Parent->AI_Array[i];
// 			if (IsValid(Ctrl) == false) {
// 				continue;
// 			}
// 		
// 			FVector Start = GetActorLocation();
// 			FVector End = Ctrl->GetPawn()->GetActorLocation() + Ctrl->GetPawn()->GetActorForwardVector() * 100;
// 		
// 			FHitResult Hit;
// 			FCollisionQueryParams Params;
// 			Params.AddIgnoredActor(this);
// 			TArray<AActor*> ActorsToIgnore;
// 			ActorsToIgnore.Add(GetOwner());
// 		
// 			bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End,
// 			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore,
// 			EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, GetWorld()->GetDeltaSeconds());
// 		
// 			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
// 			//FString::Printf(TEXT("bool %d"), bResult));
// 		
// 			if (bResult == false) {
// 				if (abs((Start - End).Rotation().Yaw - Ctrl->GetPawn()->GetActorRotation().Yaw) < Ctrl->OwnerPawn->Sense_Sight_Degrees) {
// 					if ((Start - End).Length() < Ctrl->OwnerPawn->Sense_Sight_Range) {
// 						return true;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	
// 	return false;
//}

void AAI_Search_Point::SetPool(AAI_Search_Point_Pool* Pool) {
	Points_Pool = Pool;
}

void AAI_Search_Point::ReleasePool() {
	if (InPool == true) {
		InPool = false;
		Points_Pool->PushPooledActor(this);
	}
}

void AAI_Search_Point::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (Cast<AMyCharacterAI>(OtherActor) != nullptr) {
		ReleasePool();
		//Destroy();
	}
	return;

}

void AAI_Search_Point::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (Cast<AMyCharacterAI>(OtherActor) != nullptr) {
		ReleasePool();
		//Destroy();
	}
	return;
}