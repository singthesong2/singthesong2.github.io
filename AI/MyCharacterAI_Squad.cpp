// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterAI_Squad.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "MyCharacterAI_Controller.h"
#include "MyCharacterAI.h"
#include "MyCharacter.h"
#include "AI_Search_Point_Manager.h"

// Sets default values
AMyCharacterAI_Squad::AMyCharacterAI_Squad() {
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root"));
	RootComponent = Root;
	
	Box1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box1"));
	Comps.Add(Box1);
	
	Box2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box2"));
	Comps.Add(Box2);

	for (INT32 i = 0; i < Comps.Num(); i += 1) {
		Comps[i]->SetupAttachment(RootComponent);
		Comps[i]->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		Active_Array.Add(false);
	}
}

void AMyCharacterAI_Squad::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AMyCharacterAI_Squad::BeginPlay() {
	Super::BeginPlay();

	for (INT32 i = 0; i < Comps.Num(); i += 1) {
		Comps[i]->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacterAI_Squad::OnOverlapBegin);
		Comps[i]->OnComponentEndOverlap.AddDynamic(this, &AMyCharacterAI_Squad::OnOverlapEnd);
	}
	
	for (INT32 i = Other_Squad.Num() - 1; i >= 0; i += -1) {
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("SameSquad")));
		if (Other_Squad[i] == this) {
			Other_Squad.RemoveAt(i);
		}
	}
}

void AMyCharacterAI_Squad::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (bBegin == false) {
		APawn* Player = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
		if (IsValid(Player) == true) {
			for (INT32 i = 0; i < Comps.Num(); i += 1) {
				FVector BoxCenter = Comps[i]->GetComponentLocation();
				FVector BoxExtent = Comps[i]->GetScaledBoxExtent();
				FBox BoxBounds = FBox(BoxCenter - BoxExtent, BoxCenter + BoxExtent);

				//bBegin = true;
				if (BoxBounds.IsInside(Player->GetActorLocation()) == true) {
					Active_Array[i] = true;
				}
			}
		}
	}
	
	for (INT32 i = Squad_Array.Num() - 1; i >= 0; i += -1) {
		// if (IsValid(Squad_Array[i]->OwnerPawn) == true) {
		// 	if (Squad_Array[i]->OwnerPawn->GetDead() == true) {
		// 		Squad_Array.RemoveAt(i);
		// 	}
		// }

		if (IsValid(Squad_Array[i]) == false) {
			Squad_Array.RemoveAt(i);
		}
	}

	for (INT32 i = 0; i < Active_Array.Num(); i += 1) {
		//GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Cyan, FString::Printf(TEXT("%d Act %d"), i, Active_Array[i]));
		Active = bAttack;
		if (Active_Array[i] == true) {
			Active = true;
			break;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Cyan, FString::Printf(TEXT("Act %d"), Active));

	if (bAttack == true) {
		for (INT32 i = 0; i < Squad_Array.Num(); i += 1) {
			if (IsValid(EnemyCharacter) == true) {
				if (Squad_Array[i]->GetCurrentState() == E_EnemyState::E_StatePatrol) {
					Squad_Array[i]->StartAlert(EnemyCharacter->GetActorLocation());
				}
			}
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, FString::Printf(TEXT("%d Act %d"), GetUniqueID(), Active));
}

void AMyCharacterAI_Squad::AddSquadArray(AMyCharacterAI_Controller* Ctrl) {
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("ADD SQUAD")));
	Squad_Array.Add(Ctrl);
}

TArray<TObjectPtr<AMyCharacterAI_Controller>> AMyCharacterAI_Squad::GetSquadArray() {
	return Squad_Array;
}

void AMyCharacterAI_Squad::RecvPointManager(AMyCharacterAI_Controller* Sender, AAI_Search_Point_Manager* Manager) {
	Manager->AI_Array = Squad_Array;
	
	for (INT32 i = 0; i < Squad_Array.Num(); i += 1) {
		if (Squad_Array[i] == Sender) {
			continue;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("RecvPointManager %d %s"), i, *Squad_Array[i]->GetName()));

		if (IsValid(Squad_Array[i]) == true) {
			Squad_Array[i]->SetSearchPointManager(Manager);
			Squad_Array[i]->StartAlert(Squad_Array[i]->GetPawn()->GetActorLocation() + Squad_Array[i]->GetPawn()->GetActorForwardVector());
		}
	}
}

void AMyCharacterAI_Squad::RecvFindEnemy(AMyCharacterAI_Controller* Sender, AMyCharacter* Enemy) {
	EnemyCharacter = Enemy;
	
	for (INT32 i = 0; i < Squad_Array.Num(); i += 1) {
		if (Squad_Array[i] == Sender) {
			continue;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("RecvFindEnemy %d %s"), i, *Squad_Array[i]->GetName()));
		
		Squad_Array[i]->GetBlackboardComponent()->SetValueAsObject(AMyCharacterAI_Controller::BB_EnemyActor, Enemy);
		Squad_Array[i]->EnemyActor = Enemy;
		Squad_Array[i]->StartAttack();
	}

	for (INT32 i = 0; i < Other_Squad.Num(); i += 1) {
		Other_Squad[i]->bAttack = true;
		Other_Squad[i]->EnemyCharacter = Enemy;
	}
}

void AMyCharacterAI_Squad::RecvLostEnemy() {
	bAttack = false;
	EnemyCharacter = nullptr;

	for (INT32 i = 0; i < Other_Squad.Num(); i += 1) {
		if (IsValid(Other_Squad[i]) == true) {
			Other_Squad[i]->bAttack = false;
		}
	}
}

bool AMyCharacterAI_Squad::SendIsAnyoneAttack() {
	for (INT32 i = 0; i < Squad_Array.Num(); i += 1) {
		if (IsValid(Squad_Array[i]) == true) {
			if (Squad_Array[i]->GetCurrentState() == E_EnemyState::E_StateAttack) {
				if (Squad_Array[i]->GetSenseCurrent() > 0) {
					return true;
				}
			}
		}
	}
	return false;
}

void AMyCharacterAI_Squad::SendAttack() {
	for (INT32 i = 0; i < Other_Squad.Num(); i += 1) {
		if (IsValid(Other_Squad[i]) == true) {
			if (Other_Squad[i]->bAttack == false) {
				Other_Squad[i]->bAttack = true;
			}
		}
	}
	
}

void AMyCharacterAI_Squad::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (Cast<AMyCharacter>(OtherActor) != nullptr) {
		for (INT32 i = 0; i < Comps.Num(); i += 1) {
			if (Comps[i] == OverlappedComp) {
				Active_Array[i] = true;
			}
		}
	}
}

void AMyCharacterAI_Squad::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (Cast<AMyCharacter>(OtherActor) != nullptr) {
		for (INT32 i = 0; i < Comps.Num(); i += 1) {
			if (Comps[i] == OverlappedComp) {
				Active_Array[i] = false;
			}
		}

		RecvLostEnemy();
	}
}