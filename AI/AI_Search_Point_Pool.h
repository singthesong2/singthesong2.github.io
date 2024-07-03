// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_Search_Point_Pool.generated.h"

UCLASS()
class SPECTER_API AAI_Search_Point_Pool : public AActor {
	GENERATED_BODY()

public:	
	AAI_Search_Point_Pool();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Point_Prepare = 500;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAI_Search_Point> Points_Class;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AAI_Search_Point>> Point_Array;
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	int32 Point_Index = 0;

	TObjectPtr<class AAI_Search_Point> PopPooledActor(AActor* Spawner);
	void PushPooledActor(TObjectPtr<class AAI_Search_Point> ActorToReturn);
	TObjectPtr<class AAI_Search_Point> ExtendPooled(AActor* Spawner);
};
