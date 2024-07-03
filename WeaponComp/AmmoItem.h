// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponComp/ItemBase.h"
#include "AmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API AAmmoItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	AAmmoItem();

protected:
	void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Calc(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoItem", meta = (AllowPrivateAccess = "true"))
	int Slot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoItem", meta = (AllowPrivateAccess = "true"))
	int Amount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AmmoItem", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AmmoItem", meta = (AllowPrivateAccess = "true"))
	class UBillboardComponent* Billboard;

	UPROPERTY()
	UMaterial* RenderingMat;

	UPROPERTY(EditAnywhere, Category = "AmmoItem", meta = (AllowPrivateAccess = "true"))
	UTexture2D* SlotTexture[3];

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AmmoItem", meta = (AllowPrivateAccess = "true"))
	class USoundCue* PickUpSound;

public:
	virtual void OnInteractRangeEntered_Implementation(AMyCharacter* Target) override;
};
