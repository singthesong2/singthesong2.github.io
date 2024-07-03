// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_Search_Point_Manager.generated.h"

UCLASS()
class SPECTER_API AAI_Search_Point_Manager : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Arrow;
	
public:	
	AAI_Search_Point_Manager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Array")
	TArray<TObjectPtr<class AMyCharacterAI_Controller>> AI_Array;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAI_Search_Point> Points_Class;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Array",  meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AAI_Search_Point>> Points;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAI_Search_Point_Pool> Points_Pool_Class;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AAI_Search_Point_Pool> Points_Pool;

	UPROPERTY()
	class UNavigationSystemV1* NavSystem;
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void ClearSearchPoint();
	void GenerateSearchPoint(int32 Size, int32 Between);
	bool PointsLeft();
	void ReleasePoint(class AAI_Search_Point* Point);
	FORCEINLINE class UArrowComponent* GetArrow() const { return Arrow; }
};
