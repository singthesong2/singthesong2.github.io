// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI_Search_Point.generated.h"

UCLASS()
class SPECTER_API AAI_Search_Point : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UArrowComponent> Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> StimuliSource;
	
public:	
	// Sets default values for this actor's properties
	AAI_Search_Point();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Array")
	TObjectPtr<class AAI_Search_Point_Manager> Parent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Array")
	bool InPool = false;
	
private:
	bool bNear = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AAI_Search_Point_Pool> Points_Pool;

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//bool FinishPoint();
	//bool CheckPoint();

	void SetPool(class AAI_Search_Point_Pool* Pool);
	void ReleasePool();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE TObjectPtr<class UArrowComponent> GetArrow() const { return Arrow; }
	//FORCEINLINE TObjectPtr<class USphereComponent> GetSphere() const { return Sphere; }
	FORCEINLINE TObjectPtr<class UAIPerceptionStimuliSourceComponent> GetStimuliSource() const { return StimuliSource; }
};
