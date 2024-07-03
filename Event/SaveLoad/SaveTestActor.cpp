// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveTestActor.h"
#include "Components/BoxComponent.h"
//#include "SaveSystem.h"

// Sets default values
ASaveTestActor::ASaveTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Collider->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ASaveTestActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorToSavable();
}

// Called every frame
void ASaveTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaveTestActor::SetActorToSavable()
{
	//USaveSystem* SaveInst = GetGameInstance()->GetSubsystem<USaveSystem>();
	//if (SaveInst != nullptr)
	//{
	//	SaveInst->SetActorToSavable(this);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Invalid save inst"));
	//}
}

void ASaveTestActor::OnInteractRangeEntered_Implementation(class AMyCharacter* Target)
{
	UE_LOG(LogTemp, Log, TEXT("Save Interaction"));
	UMaterialInstanceDynamic* mat = (UMaterialInstanceDynamic*)Mesh->GetMaterial(0);
	if (mat)
	{
		this->Mesh->SetMaterial(0, UMaterialInstanceDynamic::Create(mat, this));
	}
}

void ASaveTestActor::OnInteractRangeExited_Implementation(AMyCharacter* Target)
{
//	USaveSystem* Inst = GetGameInstance()->GetSubsystem<USaveSystem>();
//	if (nullptr == Inst)
//	{
//		UE_LOG(LogTemp, Log, TEXT("Invalid savesystem instance"));
//		return;
//	}
//	Inst->SetActorToSavable(this);
}

//void ASaveTestActor::SetActorToSavable(const UObject* WorldContextObject)
//{
//	Super::SetActorToSavable(this);
//}