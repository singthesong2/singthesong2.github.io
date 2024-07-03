// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIEnum.h"
#include "CharacterAISpawner.generated.h"

UCLASS()
class SPECTER_API ACharacterAISpawner : public AActor {
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACharacterAISpawner();

private:
	// 생성된 AI의 행동입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> AI_Behavior;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AMyCharacterAI>> AI_Array;

public:
	// 생성할 AI 캐릭터입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AMyCharacterAI> AI_Character;

	// 생성된 AI의 스쿼드입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class AMyCharacterAI_Squad> AI_Squad;
	
	// 생성된 AI의 경로입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AAI_Patrol_Path> AI_Patrol_Spline;

	// AI의 최대 생성 갯수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AI_Count = 1;

	// AI의 시작 생성 간격입니다. (단위 : 초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AI_Begin = 5.0f;
	// AI의 생성 간격입니다. (단위 : 초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AI_Between = 5.0f;
	float AI_Timer = 0.0f;

	// AI의 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_EnemyState AI_State = E_EnemyState::E_StatePatrol;
	// AI의 공격시간 만료여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AI_Attack_Timeout = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Spawn();
};
