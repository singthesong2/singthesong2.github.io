// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_GetSplineLocation.h"

#include "AI_Patrol_Path.h"
#include "MyCharacterAI.h"
#include "MyCharacterAI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SplineComponent.h"

UBTTask_GetSplineLocation::UBTTask_GetSplineLocation() {
	NodeName = TEXT("GetSplineLocation");
}

EBTNodeResult::Type UBTTask_GetSplineLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//auto const ControllingPawn = Cast<ACharacter_AI>(OwnerComp.GetAIOwner()->GetPawn());
	auto const ControllingPawn = Cast<AMyCharacterAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(ControllingPawn) == true)	{
		if (IsValid(ControllingPawn->Patrol_Spline) == true) {
			TObjectPtr<USplineComponent> Spline = ControllingPawn->Patrol_Spline->GetSpline();
			if (IsValid(Spline) == true) {

				// ControllingPawn->Patrol_Spline_Position += 2.5;
				// if (ControllingPawn->Patrol_Spline_Position > Spline->GetSplineLength()) {
				// 	ControllingPawn->Patrol_Spline_Position = 0;
				// }
				// FVector Position = Spline->GetLocationAtDistanceAlongSpline(ControllingPawn->Patrol_Spline_Position, ESplineCoordinateSpace::World);
				
				float CurrentDistance = Spline->FindInputKeyClosestToWorldLocation(ControllingPawn->GetActorLocation());
				float Along = Spline->GetDistanceAlongSplineAtSplineInputKey(CurrentDistance) + 100;
				if (Along > Spline->GetSplineLength()) {
					Along += -Spline->GetSplineLength();
				}
				FVector Position = Spline->GetLocationAtDistanceAlongSpline(Along, ESplineCoordinateSpace::World);

				FVector StartLocation = Position;
				FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, 1000.0f);
				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(OwnerComp.GetAIOwner()->GetPawn());
				if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params)) {
					Position = HitResult.Location;
				}
				
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyCharacterAI_Controller::BB_NextVector, Position);
				//DrawDebugPoint(GetWorld(), Position, 10, FColor::Black, false, 5, 0);
				
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
