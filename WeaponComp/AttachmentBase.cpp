// Fill out your copyright notice in the Description page of Project Settings.


#include "AttachmentBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAttachmentBase::AAttachmentBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(DefaultSceneComponent);
	

	AttachmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachmentMesh"));
	AttachmentMesh->SetupAttachment(DefaultSceneComponent);
}

// Called when the game starts or when spawned
void AAttachmentBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAttachmentBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAttachmentBase::Init_OnCreate()
{
	AttachmentMesh->SetCollisionProfileName("NoCollision");
}