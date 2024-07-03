// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAISpawner.h"

#include "MyCharacter.h"
#include "MyCharacterAI.h"
#include "MyCharacterAI_Controller.h"
#include "MyCharacterAI_Squad.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACharacterAISpawner::ACharacterAISpawner() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterAISpawner::BeginPlay() {
	Super::BeginPlay();
	AI_Timer = -AI_Begin + AI_Between;
}

// Called every frame
void ACharacterAISpawner::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	AI_Timer += DeltaTime;
	AI_Timer = FMath::Clamp(AI_Timer, -AI_Begin, AI_Between);

	for (int32 i = AI_Array.Num() - 1; i >= 0; i += -1) {
		if (IsValid(AI_Array[i]) == false) {
			AI_Array.RemoveAt(i);
		}
		else {
			if (AI_Array[i]->GetDead() == true) {
				AI_Array.RemoveAt(i);
			}
		}
	}
	
	if (AI_Timer >= AI_Between) {
		if (AI_Array.Num() < AI_Count) {
			AI_Timer = 0;
			Spawn();
		}
	}
}

void ACharacterAISpawner::Spawn() {
	APawn* Obj;
	Obj = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), AI_Character, AI_Behavior, GetActorLocation(), GetActorRotation(), false, NULL);
	if (Obj != nullptr) {
		TObjectPtr<AMyCharacterAI> AI = Cast<AMyCharacterAI>(Obj);
		AI_Array.Add(AI);

		// if (IsValid(AI_Patrol_Spline) == true) {
		// 	
		// }
		AI->Patrol_Type = E_EnemyPatrolType::Patrol_Spline;
		
		AI->Patrol_Spline = AI_Patrol_Spline;
		AI->Combat_Timeout = AI_Attack_Timeout;
		
		TObjectPtr<AMyCharacterAI_Controller> AICtrl = Cast<AMyCharacterAI_Controller>(AI->GetController());
		if (IsValid(AICtrl) == true) {
			if (IsValid(AI_Squad) == true) {
				AI->Squad = AI_Squad;
				AI->Alert_Size = AI_Squad->Search_Size;
				AI->Alert_Between = AI_Squad->Search_Between;
				AICtrl->Squad = AI_Squad;
				AI_Squad->AddSquadArray(AICtrl);
			}

			AICtrl->GetBlackboardComponent()->SetValueAsEnum(AMyCharacterAI_Controller::BB_CurrentPatrol, (uint8)E_EnemyPatrolType::Patrol_Spline);
			if (AI_State == E_EnemyState::E_StateAttack) {
				AMyCharacter* tChar = Cast<AMyCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
				AICtrl->GetBlackboardComponent()->SetValueAsObject(AMyCharacterAI_Controller::BB_EnemyActor, tChar);
				AICtrl->EnemyActor = tChar;
				AICtrl->StartAttack();
				if (IsValid(AI_Squad) == true) {
					AI_Squad->RecvFindEnemy(AICtrl, tChar);
				}
			}
		}
	}
}
