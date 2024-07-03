// Fill out your copyright notice in the Description page of Project Settings.

#include "AICoverPoint.h"

#include "Kismet/GameplayStatics.h"

#include "CoverSystem.h"
#include "MyCharacterAI_Controller.h"

AAICoverPoint::AAICoverPoint() {
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

void AAICoverPoint::BeginPlay() {
	Super::BeginPlay();
	PrimaryActorTick.TickInterval = Cover_Interval;

	Cover_System = ACoverSystem::GetCoverSystem(GetWorld());
}

void AAICoverPoint::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	APawn* Player = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	if (IsValid(Player) == true) {
		RefreshCover(Player->GetActorLocation() + FVector(0.0f, 0.0f, 80.0f));
		//DrawDebugSphere(GetWorld(), Player->GetActorLocation(), 50, 12, FColor::Yellow, false, DeltaSeconds, 0, 3);
	}
	
	for (auto& tCover : Cover_List) {
		if (Cover_Usable.Find(tCover.Handle) != nullptr) {
			//DrawDebugSphere(GetWorld(), tCover.Data.Location, 50, 12, FColor::Yellow, false, DeltaSeconds, 0, 3);
		}
	}
}

void AAICoverPoint::RefreshCover(FVector Location) {
	FBoxCenterAndExtent Ext = FBoxCenterAndExtent(Location, FVector(3000, 3000, 3000));
	TMap<FCoverHandle, TObjectPtr<AMyCharacterAI_Controller>> tempMap = Cover_Usable;
	
	if (IsValid(Cover_System) == false) {
		return;
	}
	Cover_System->GetCoversAndDataWithinBounds(Ext,Cover_List);
	
	Cover_Usable.Reset();
	for (auto& tCover : Cover_List) {
		bool tResult = false;
		float tAngle = FMath::DegreesToRadians(90.0f);
		
		FHitResult FilterHit;
		FVector FilterStart = tCover.Data.Location;
		FVector FilterEnd = Location;

		GetWorld()->LineTraceSingleByChannel(FilterHit, FilterStart, FilterEnd, ECollisionChannel::ECC_GameTraceChannel2);
		if (Cover_Debug) DrawDebugLine(GetWorld(), FilterStart, FilterHit.bBlockingHit == true ? FilterHit.Location : FilterEnd, FilterHit.bBlockingHit == true ? FColor::Red : FColor::Green, false, Cover_Interval, 0, 3);
		if (FilterHit.bBlockingHit == false) {
			continue;
		}
		
		if (tCover.Data.bCrouchedCover == true) {
			GetWorld()->LineTraceSingleByChannel(FilterHit, FilterStart, FilterEnd, ECollisionChannel::ECC_GameTraceChannel2);
			if (Cover_Debug) DrawDebugLine(GetWorld(), FilterStart, FilterHit.bBlockingHit == true ? FilterHit.Location : FilterEnd, FilterHit.bBlockingHit == true ? FColor::Red : FColor::Green, false, Cover_Interval, 0, 3);
			if (FilterHit.bBlockingHit == false) {
				tResult = true;
				tCover.Data.bCrouchedCover = true;
			}
			else {
				tCover.Data.bCrouchedCover = false;
			}
		}
		if (tCover.Data.bFrontCoverCrouched == true) {
			FilterStart = tCover.Data.Location + FVector(0.0f, 0.0f, 80.0f) + FVector(0, 0, 60.0f);
			GetWorld()->LineTraceSingleByChannel(FilterHit, FilterStart, FilterEnd, ECollisionChannel::ECC_GameTraceChannel2);
			if (Cover_Debug) DrawDebugLine(GetWorld(), FilterStart, FilterHit.bBlockingHit == true ? FilterHit.Location : FilterEnd, FilterHit.bBlockingHit == true ? FColor::Red : FColor::Green, false, Cover_Interval, 0, 3);
			if (FilterHit.bBlockingHit == false) {
				tResult = true;
				tCover.Data.bFrontCoverCrouched = true;
			}
			else {
				tCover.Data.bFrontCoverCrouched = false;
			}
		}
		// if (tCover.Data.bLeftCoverCrouched == true) {
		// 	float tx = tCover.Data.DirectionToWall.X * FMath::Cos(-tAngle) - tCover.Data.DirectionToWall.Y * FMath::Sin(-tAngle);
		// 	float ty = tCover.Data.DirectionToWall.X * FMath::Sin(-tAngle) + tCover.Data.DirectionToWall.Y * FMath::Cos(-tAngle);
		// 	FilterStart = tCover.Data.Location + FVector(0.0f, 0.0f, 80.0f) + FVector(tx, ty, tCover.Data.DirectionToWall.Z) * 60;
		// 	GetWorld()->LineTraceSingleByChannel(FilterHit, FilterStart, FilterEnd, ECollisionChannel::ECC_GameTraceChannel2);
		// 	if (Cover_Debug) DrawDebugLine(GetWorld(), FilterStart, FilterHit.bBlockingHit == true ? FilterHit.Location : FilterEnd, FilterHit.bBlockingHit == true ? FColor::Red : FColor::Green, false, Cover_Interval, 0, 3);
		// 	if (FilterHit.bBlockingHit == false) {
		// 		tResult = true;
		// 		tCover.Data.bLeftCoverCrouched = true;
		// 	}
		// 	else {
		// 		tCover.Data.bLeftCoverCrouched = false;
		// 	}
		// }
		// if (tCover.Data.bLeftCoverStanding == true) {
		// 	float tx = tCover.Data.DirectionToWall.X * FMath::Cos(-tAngle) - tCover.Data.DirectionToWall.Y * FMath::Sin(-tAngle);
		// 	float ty = tCover.Data.DirectionToWall.X * FMath::Sin(-tAngle) + tCover.Data.DirectionToWall.Y * FMath::Cos(-tAngle);
		// 	FilterStart = tCover.Data.Location + FVector(0.0f, 0.0f, 180.0f) + FVector(tx, ty, tCover.Data.DirectionToWall.Z) * 60;
		// 	GetWorld()->LineTraceSingleByChannel(FilterHit, FilterStart, FilterEnd, ECollisionChannel::ECC_GameTraceChannel2);
		// 	if (Cover_Debug) DrawDebugLine(GetWorld(), FilterStart, FilterHit.bBlockingHit == true ? FilterHit.Location : FilterEnd, FilterHit.bBlockingHit == true ? FColor::Red : FColor::Green, false, Cover_Interval, 0, 3);
		// 	if (FilterHit.bBlockingHit == false) {
		// 		tResult = true;
		// 		tCover.Data.bLeftCoverStanding = true;
		// 	}
		// 	else {
		// 		tCover.Data.bLeftCoverStanding = false;
		// 	}
		// }
		// if (tCover.Data.bRightCoverCrouched == true) {
		// 	float tx = tCover.Data.DirectionToWall.X * FMath::Cos(tAngle) - tCover.Data.DirectionToWall.Y * FMath::Sin(tAngle);
		// 	float ty = tCover.Data.DirectionToWall.X * FMath::Sin(tAngle) + tCover.Data.DirectionToWall.Y * FMath::Cos(tAngle);
		// 	FilterStart = tCover.Data.Location + FVector(0.0f, 0.0f, 80.0f) + FVector(tx, ty, tCover.Data.DirectionToWall.Z) * 60;
		// 	GetWorld()->LineTraceSingleByChannel(FilterHit, FilterStart, FilterEnd, ECollisionChannel::ECC_GameTraceChannel2);
		// 	if (Cover_Debug) DrawDebugLine(GetWorld(), FilterStart, FilterHit.bBlockingHit == true ? FilterHit.Location : FilterEnd, FilterHit.bBlockingHit == true ? FColor::Red : FColor::Green, false, Cover_Interval, 0, 3);
		// 	if (FilterHit.bBlockingHit == false) {
		// 		tResult = true;
		// 		tCover.Data.bRightCoverCrouched = true;
		// 	}
		// 	else {
		// 		tCover.Data.bRightCoverCrouched = false;
		// 	}
		// }
		// if (tCover.Data.bRightCoverStanding == true) {
		// 	float tx = tCover.Data.DirectionToWall.X * FMath::Cos(tAngle) - tCover.Data.DirectionToWall.Y * FMath::Sin(tAngle);
		// 	float ty = tCover.Data.DirectionToWall.X * FMath::Sin(tAngle) + tCover.Data.DirectionToWall.Y * FMath::Cos(tAngle);
		// 	FilterStart = tCover.Data.Location + FVector(0.0f, 0.0f, 180.0f) + FVector(tx, ty, tCover.Data.DirectionToWall.Z) * 60;
		// 	GetWorld()->LineTraceSingleByChannel(FilterHit, FilterStart, FilterEnd, ECollisionChannel::ECC_GameTraceChannel2);
		// 	if (Cover_Debug) DrawDebugLine(GetWorld(), FilterStart, FilterHit.bBlockingHit == true ? FilterHit.Location : FilterEnd, FilterHit.bBlockingHit == true ? FColor::Red : FColor::Green, false, Cover_Interval, 0, 3);
		// 	if (FilterHit.bBlockingHit == false) {
		// 		tResult = true;
		// 		tCover.Data.bRightCoverStanding = true;
		// 	}
		// 	else {
		// 		tCover.Data.bRightCoverStanding = false;
		// 	}
		// }


		
		if (tResult == true) {
			bool tFlag = false;
			for (auto& tCtrl : tempMap) {
				FCoverHandle tHandle = tCover.Handle;
				if (tCtrl.Key == tCover.Handle) {
					if (IsValid(tCtrl.Value) == true) {
						tFlag = true;
						Cover_Usable.Add(tCover.Handle, tCtrl.Value);
						//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("a %s"), *tCtrl.Value.GetName()));
						break;
					}
				}
			}
			if (tFlag == false) {
				Cover_Usable.Add(tCover.Handle, nullptr);
			}
		}
		else {
			tCover.Data.bCrouchedCover = false;
			tCover.Data.bFrontCoverCrouched = false;
			tCover.Data.bLeftCoverCrouched = false;
			tCover.Data.bLeftCoverStanding = false;
			tCover.Data.bRightCoverCrouched = false;
			tCover.Data.bRightCoverStanding = false;
		}
		
		// TObjectPtr<AMyCharacterAI_Controller> Ctrl = tempMap.FindRef(tCover.Handle);
		// if (Ctrl) {
		// 	Cover_Usable.Add(tCover.Handle, Ctrl);
		// }
		// else {
		// 	Cover_Usable.Add(tCover.Handle, false);
		// }
	}
}

bool AAICoverPoint::PickCover(FCover &Cover, TObjectPtr<AMyCharacterAI_Controller> Controller, float Length) {
	FCover tResultCover;
	float tLength = Length;
	
	for (auto& tUsable : Cover_Usable) {
		if ((tUsable.Value) == nullptr) {
			FCover tCover;
			tCover.Handle = tUsable.Key;

			if (Cover_System->GetCoverData(tCover.Handle, tCover.Data)) {
				if (tLength > (tCover.Data.Location - Controller->GetPawn()->GetActorLocation()).Length()) {
					tLength = (tCover.Data.Location - Controller->GetPawn()->GetActorLocation()).Length();
					tResultCover = tCover;
				}
			}
		}
		else if ((tUsable.Value) == Controller) {
			tUsable.Value = nullptr;
		}
	}

	if (tResultCover.Handle.IsValid() == true) {
		TObjectPtr<AMyCharacterAI_Controller>* tController = Cover_Usable.Find(tResultCover.Handle);
		*tController = Controller;
		Cover = tResultCover;
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("b %s"), *tController->GetName()));

		return true;
	}

	return false;
}


