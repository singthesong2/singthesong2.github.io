// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "MyCharacterAI_Squad.generated.h"

UCLASS()
class SPECTER_API AMyCharacterAI_Squad : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBillboardComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> Box1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> Box2;
	
public:	
	// Sets default values for this actor's properties
	AMyCharacterAI_Squad();
	
private:
	UPROPERTY()
	TObjectPtr<class AMyCharacter> EnemyCharacter;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> Comps;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AMyCharacterAI_Controller>> Squad_Array;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AAI_Search_Point_Manager> Search_Point_Manager;

public:
	// 다른 스쿼드를 설정합니다. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<TObjectPtr<class AMyCharacterAI_Squad>> Other_Squad;
	// 수색의 EQS 범위를 설정합니다. 스쿼드의 포함된 AI들의 수색 EQS 범위가 덮어씌워집니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default");
	int32 Search_Size = 10;
	// 수색의 EQS 간격을 설정합니다. 스쿼드의 포함된 AI들의 수색 EQS 간격이 덮어씌워집니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default");
	int32 Search_Between = 400;

	bool bBegin = false;
	bool bAttack = false;
	float Attack = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	TArray<bool> Active_Array;
	bool Active = false;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void AddSquadArray(AMyCharacterAI_Controller* Ctrl);
	TArray<TObjectPtr<class AMyCharacterAI_Controller>> GetSquadArray();
	
	void RecvPointManager(class AMyCharacterAI_Controller* Sender, class AAI_Search_Point_Manager*);
	void RecvFindEnemy(class AMyCharacterAI_Controller* Sender, class AMyCharacter* Enemy);
	void RecvLostEnemy();
	bool SendIsAnyoneAttack();

	void SendAttack();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
