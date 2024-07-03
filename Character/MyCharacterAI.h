// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacterBase.h"
#include "AIEnum.h"
#include "Components/TimeLineComponent.h"
#include "../WeaponComp/WeaponToOwner.h"
#include "MyCharacterAI.generated.h"

UCLASS()
class SPECTER_API AMyCharacterAI : public AMyCharacterBase, public IWeaponToOwner {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> WidgetIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> AimCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> CloseBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWeaponComponent> Weapon_Component;

public:
	AMyCharacterAI();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AMyCharacterAI_Controller> AI_Controller;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TArray<TSubclassOf<class AGunBase>> GunSlot;
	
	// 플레이어와 일정거리 이하면 활성화되는 범위입니다. 스쿼드가 있는 경우 스쿼드의 활성화 범위가 우선시 됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	float Active_Range = 3000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	bool Active_Check = false;
	
	// 다른 AI들과 협동할 스쿼드를 설정합니다. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Squad")
	TObjectPtr<class AMyCharacterAI_Squad> Squad;

	// 수색의 EQS 존재 시간입니다. (단위 : 초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Variable")
	float Sense_Alert_Max = 10.0f;
	// 플레이어 탐지시 수치의 증가값입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Variable")
	float Sense_Search_Increase = 1.0f;
	// 플레이어 탐지의 최대 수치값입니다. 수치가 최대가 되면 공격 상태로 전환합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Variable")
	float Sense_Search_Max = 100.0f;
	// 공격의 최대 유지 시간입니다. (단위 : 초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Variable")
	float Sense_Attack_Max = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Perception")
	bool Sense_Sight_Scope = false;
	// AI 시각의 거리입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Perception")
	float Sense_Sight_Range = 500.0f;
	// AI 시각의 각도입니다. (제한범위 : 0~180)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Perception")
	float Sense_Sight_Degrees = 100.0f;
	// AI 시각의 유지 시간입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Perception")
	float Sense_Sight_Age = 0.0f;
	// AI 청각의 거리입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Perception")
	float Sense_Hearing_Range = 1000.0f;
	// AI 청각의 유지 시간입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Perception")
	float Sense_Hearing_Age = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Perception")
	float Distance_Short = 300;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Perception")
	float Distance_Middle = 600;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Perception")
	float Distance_Long = 900;

	// 순찰 유형을 설정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Patrol")
	E_EnemyPatrolType Patrol_Type = E_EnemyPatrolType::Patrol_Stay;
	// 순찰 스플라인을 설정합니다. 순찰 유형이 지점 혹은 스플라인일때만 작동합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Patrol|Spline")
	TObjectPtr<class AAI_Patrol_Path> Patrol_Spline;
	// 순찰 스플라인의 이동 초기 지점을 설정합니다. 순찰 유형이 지점일때만 작동합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Patrol|Spline")
	int32 Patrol_Spline_Init = 0;
	float Patrol_Spline_Position = 0;
	
	// 무작위 순찰의 이동 범위를 설정합니다. 순찰 유형이 무작위일때만 작동합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Patrol|Random")
	float Patrol_Random_Range = 500.0f;

	// 수색의 EQS 범위를 설정합니다. 스쿼드가 있는 경우 스쿼드의 EQS 범위가 우선시 됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Alert")
	int32 Alert_Size = 5;
	// 수색의 EQS 간격를 설정합니다. 스쿼드가 있는 경우 스쿼드의 EQS 간격이 우선시 됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Alert")
	int32 Alert_Between = 400;

	// 미완성. 탐색 방식
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Search")
	E_EnemySearchType Search_Type = E_EnemySearchType::Search_Normal;

	// 미완성. 공격 방식
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Attack")
	E_EnemyCombatType Combat_Type = E_EnemyCombatType::Combat_Close;

	// 공격 시간이 만료되면 공격 상태를 해체합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Attack")
	bool Combat_Timeout	= true;

	// AI가 사용하는 커브값입니다. 무조건 존재해야합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Curve")
	TObjectPtr<class UCurveFloat> TL_Look_Curve;
	
protected:
	// 순찰 스플라인의 이동 방향을 설정합니다. 체크 해제면 역방향으로 이동합니다. 순찰 유형이 스플라인일때만 작동합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Patrol|Spline")
	bool Patrol_Forward = true;
	
	float Death_Time = 10.0f;
	int32 Patrol_Spline_Index = 0;
	
	FVector Patrol_Start_Location = FVector(0.0f, 0.0f, 0.0f);
	FVector Patrol_Start_Forward = FVector(0.0f, 0.0f, 0.0f);
	FRotator Patrol_Start_Rotator = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY()
	AActor* Actor_Overlap;
	UPROPERTY()
	FTimerHandle Timer_Overlap;
	
	UPROPERTY()
	FTimeline TL_Look;
	UPROPERTY()
	FOnTimelineFloat TL_Look_UpdateDelegate;
	UPROPERTY()
	FOnTimelineEvent TL_Look_FinishDelegate;
	FRotator TL_Look_Rotator_Begin = FRotator(0.0f, 0.0f, 0.0f);
	FRotator TL_Look_Rotator_End = FRotator(0.0f, 0.0f, 0.0f);
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual FVector FixTrainLocation(FVector Point) override;

	UFUNCTION()
	FTransform GetStartPosition();
	FTransform GetNextPatrolPosition();
	
	UFUNCTION()
	virtual void LookAtPoint(FVector Point, float PlayRate);

	virtual void ActiveDead() override;

	UFUNCTION()
	void TL_Look_UpdateFunc(float Value);
	UFUNCTION()
	void TL_Look_FinishFunc();
	
	void ShowIcon(E_EnemyState State);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAmsalMontage();

	//UFUNCTION(BlueprintCallable)
	virtual void GetDamage(float Damage) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Test")
	void GunFire();
	virtual void GunFire_Implementation();
	
	void CheckCover();

	void CheckOverlap();
	
	//퍼셉션 소켓 참조 메쉬
	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;
	
	//재장전 호출
	virtual void PlayReloadAnimation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "PlayReloadAnimation", ScriptName = "PlayReloadAnimation"))
	void K2_PlayReloadAnimation();
	virtual void K2_PlayReloadAnimation_Implementation() {};

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE TObjectPtr<class UWidgetComponent> GetWidgetIcon() const { return WidgetIcon; }
	FORCEINLINE TObjectPtr<class UCameraComponent> GetAimCamera() const { return AimCamera; }
	FORCEINLINE TObjectPtr<class UBoxComponent> GetCloseBox() const { return CloseBox; }
	FORCEINLINE TObjectPtr<class UWeaponComponent> GetWeaponComponent() const { return Weapon_Component; }

	
};
