// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../WeaponComp/MyAniTable.h"
#include "Animation/AnimInstance.h"
#include "Characte_Ani.generated.h"

UCLASS()
class SPECTER_API UCharacte_Ani : public UAnimInstance
{
	GENERATED_BODY()

	UCharacte_Ani();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	bool Is_Crouch = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	bool Is_Sprint = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	float Forward = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	float Left = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position", meta = (AllowPrivateAccess = "true"))
	float Player_Yaw = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position", meta = (AllowPrivateAccess = "true"))
	float Player_Pitch = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position", meta = (AllowPrivateAccess = "true"))
	bool MoveDirection = false;

public:
	class UDataTable* AniTable;

	void PlayMontage(FName arg);

	UFUNCTION()
	void AnimNotify_JumpFlag();

	UFUNCTION()
	void AnimNotify_Death();
};
