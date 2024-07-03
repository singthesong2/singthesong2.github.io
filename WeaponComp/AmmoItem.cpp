// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComp/AmmoItem.h"

#include "Train_Between.h"
#include "Train_Rail.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "../Character/MyCharacter.h"

AAmmoItem::AAmmoItem()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("/Game/Default/ThirdPersonKit/Meshes/TPSKitProps/SciFiAmmoBox/SM_SciFiAmmoBox_01_01.SM_SciFiAmmoBox_01_01"));
	static ConstructorHelpers::FObjectFinder<UMaterial> RenderMat(TEXT("/Script/Engine.Material'/Game/WeaponSystem/Item/M_HighLightMat.M_HighLightMat'"));

	if (RenderMat.Succeeded())
	{
		RenderingMat = RenderMat.Object;
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(TrainComponent);
	Mesh->SetOverlayMaterial(RenderingMat);

	if (SM.Succeeded())
	{
		Mesh->SetStaticMesh(SM.Object);
	}

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(TrainComponent);
	Billboard->AddRelativeLocation(FVector(0.f, 0.f, 100.f));
	Billboard->SetWorldScale3D(FVector(5.f, 5.f, 5.f));
	Billboard->SetHiddenInGame(false);
}

void AAmmoItem::BeginPlay()
{
	Super::BeginPlay();

	UTexture2D* Tex = SlotTexture[Slot];

	Billboard->SetSprite(Tex);
}

void AAmmoItem::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void AAmmoItem::Calc(float DeltaTime) {
	FVector CompLocation = GetActorLocation();
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	if (IsValid(Actor_Train) == true) {
		FRotator tVecRot = Actor_Train->Rail_Train->GetActorRotation() + FRotator(0, 90, 0);
		FVector tVec = Actor_Train->Rail_Train->GetActorLocation() - tVecRot.RotateVector(Actor_Train->GetActorLocation() - GetActorLocation());
		FRotator tRot;
		tRot.Roll = GetActorRotation().Roll;
		tRot.Pitch = GetActorRotation().Pitch;
		tRot.Yaw = Actor_Train->Rail_Train->GetActorRotation().Yaw + GetActorRotation().Yaw + 90;
		TrainComponent->SetWorldLocation(tVec, false, nullptr, ETeleportType::TeleportPhysics);
		TrainComponent->SetWorldRotation(tRot, false, nullptr, ETeleportType::TeleportPhysics);
		CompLocation = TrainComponent->GetComponentLocation();
	}
	else {
		TrainComponent->SetWorldLocation(GetActorLocation());
	}
	
	if (GetWorld()->LineTraceSingleByChannel(Hit, CompLocation, CompLocation - FVector(0, 0, 100), ECollisionChannel::ECC_GameTraceChannel2, CollisionParams)) {
		//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, FString::Printf(TEXT("%s %s"), *this->GetName(), *Hit.GetActor()->GetName()));
		if (Hit.GetActor()->IsA(ATrain_Between::StaticClass()) == true) {
			if (Actor_Train != Hit.GetActor()) {
				Actor_Train = Cast<ATrain_Between>(Hit.GetActor());
			}
		}
		else if (Hit.GetActor()->IsA(ATrain_Rail::StaticClass()) == true) {
			TObjectPtr<ATrain_Rail> TR = Cast<ATrain_Rail>(Hit.GetActor());
			if (Actor_Train != TR->Train_Between) {
				FRotator RotTrain = TR->GetActorRotation() + FRotator(0, 90, 0);
				RotTrain *= -1;
				FVector Reposition = TR->Train_Between->GetActorLocation() - RotTrain.RotateVector(TR->GetActorLocation() - TrainComponent->GetComponentLocation());
				SetActorLocation(Reposition, false, nullptr, ETeleportType::None);
				Actor_Train = TR->Train_Between;
				//DrawDebugCircle(GetWorld(), Reposition, 1000, 20, FColor::Green, false, 100, 0, 10);
	
				FRotator tVecRot = Actor_Train->Rail_Train->GetActorRotation() + FRotator(0, 90, 0);
				FVector tVec = Actor_Train->Rail_Train->GetActorLocation() - tVecRot.RotateVector(Actor_Train->GetActorLocation() - GetActorLocation());
				FRotator tRot;
				tRot.Roll = GetActorRotation().Roll;
				tRot.Pitch = GetActorRotation().Pitch;
				tRot.Yaw = Actor_Train->Rail_Train->GetActorRotation().Yaw + GetActorRotation().Yaw + 90;
				TrainComponent->SetWorldLocation(tVec, false, nullptr, ETeleportType::TeleportPhysics);
				TrainComponent->SetWorldRotation(tRot, false, nullptr, ETeleportType::TeleportPhysics);
			}
		}
	}
	
	if (IsValid(PreActor) == false) {
		if (IsValid(Actor_Train) == true) {
			TObjectPtr<ATrain_Rail> PreTick = Actor_Train->Rail_Train;
			while(true) {
				if (IsValid(PreTick) == true) {
					if (IsValid(PreTick->Back_Train) == true) {
						PreTick = PreTick->Back_Train;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			PreActor = PreTick;
			AddTickPrerequisiteActor(PreActor);
		}
	}
}

void AAmmoItem::OnInteractRangeEntered_Implementation(AMyCharacter* Target)
{
	if (IsValid(Target))
	{
		Target->AddAmmo(Slot, Amount);
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("Ammo Add"));
		if (IsValid(PickUpSound))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpSound, AActor::GetActorLocation());
		}

		AActor::Destroy();
	}
}
