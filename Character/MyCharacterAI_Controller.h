// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIEnum.h"
#include "AIController.h"
#include "CoverSystemPublicData.h"
#include "Perception/AIPerceptionTypes.h"
#include "MyCharacterAI_Controller.generated.h"

UCLASS()
class SPECTER_API AMyCharacterAI_Controller : public AAIController {
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = "AIPerception")
	TObjectPtr<class UAIPerceptionComponent> AIPerception;

	UPROPERTY(VisibleDefaultsOnly, Category = "AIPerception")
	TObjectPtr<class UAISenseConfig_Sight> AISense_Sight;
	UPROPERTY(VisibleDefaultsOnly, Category = "AIPeception")
	TObjectPtr<class UAISenseConfig_Hearing> AISense_Hearing;
	UPROPERTY(VisibleDefaultsOnly, Category = "AIPerception")
	TObjectPtr<class UAISenseConfig_Damage> AISense_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBlackboardData> BBAsset;

public:
	AMyCharacterAI_Controller();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AMyCharacterAI_Squad> Squad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AMyCharacterAI> OwnerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AMyCharacterAI_Train> OwnerPawnTrain;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AMyCharacter> EnemyActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AAICoverPoint> CoverManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FCover CoverPoint;

	int ViewCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Perception_Sight = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MontageNum = 0;

	UPROPERTY()
	FTimerHandle Timer_Amsal;
	UPROPERTY()
	FTimerHandle Timer_GunFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	bool Alert_Aim = false;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAI_Search_Point_Manager> Search_Point_Manager_Class;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AAI_Search_Point_Manager> Search_Point_Manager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	E_EnemyState Current_State = E_EnemyState::E_StateOff;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	E_EnemyCombatCoverType Current_Cover = E_EnemyCombatCoverType::Cover_None;
	FAIStimulus Current_Stimulus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	float Active_Range = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	float Sense_Current = 0.0f;
	float Sense_Alert_Max = 10.0f;
	float Sense_Search_Increase = 1.0f;
	float Sense_Search_Max = 100.0f;
	float Sense_Attack_Max = 5.0f;
	float Sense_Range = 0.0f;

	bool Sense_Sight_Scope = false;
	float Sense_Sight_Scope_Range = 500.0f;
	float Sense_Sight_Range = 500.0f;
	float Sense_Sight_Degrees = 100.0f;
	float Sense_Sight_Age = 0.0f;
	float Sense_Hearing_Range = 1000.0f;
	float Sense_Hearing_Age = 0.0f;

	float Distance_Short = 100.0f;
	float Distance_Middle = 200.0f;
	float Distance_Long = 300.0f;

	float Walk_Speed = 600.0f;

	float Temp_Max = 2;
	float Temp_Cur = 0;

	UPROPERTY()
	FTimerHandle Timer_View;

public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Destroyed() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	UFUNCTION()
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus const Stimulus);

	UFUNCTION(BlueprintCallable)
	void StateChange(E_EnemyState NewState);

	UFUNCTION(BlueprintImplementableEvent)
	void Widget_Call();

	UFUNCTION(BlueprintImplementableEvent)
	void ReMoveWidget();

	void StartPatrol();
	void StartAlert(FVector vec);
	void StartSearch();
	void StartAttack();

	void SetSearchPointManager(class AAI_Search_Point_Manager* Manager);
	void RunSearchPointManager(FVector Point);

	E_EnemyState GetCurrentState() const;
	void SetCurrentState(E_EnemyState NewState);
	
	E_EnemyCombatCoverType GetCurrentCover() const;
	void SetCurrentCover(E_EnemyCombatCoverType NewCover);
	
	float GetSenseCurrent() const;
	void SetSenseCurrent(float Input);

	TTuple<float, float> GetSightConfig();

	UFUNCTION(BlueprintPure)
	bool GetTarget();

	UFUNCTION()
	void GetAmsal(APawn* InPawn);

	UFUNCTION(BlueprintCallable)
	void FinishAmsal();

	UFUNCTION()
	void LookPlayer();
	
	UFUNCTION()
	void StartWeaponFire();
	UFUNCTION()
	void DoWeaponFire();
	UFUNCTION()
	void StopWeaponFire();

	FVector CalcLocation();   

	void UpdateSight(float Range, float Angle);

	UFUNCTION()
	void RSPM(FVector Location);

public:
	static const FName BB_SelfActor;
	static const FName BB_EnemyActor;
	static const FName BB_CurrentState;
	static const FName BB_CurrentCombat;
	static const FName BB_CurrentCover;
	static const FName BB_CurrentPatrol;
	static const FName BB_NextVector;
	static const FName BB_NextRotator;
	static const FName BB_StartVector;
	static const FName BB_StartRotator;
	static const FName BB_SearchSize;

	FORCEINLINE class UAIPerceptionComponent* GetAIPerception() const { return AIPerception; }
};
