// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characte_AI_Ani.generated.h"

UCLASS()
class SPECTER_API UCharacte_AI_Ani : public UAnimInstance
{
	GENERATED_BODY()

	UCharacte_AI_Ani();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	bool Is_Crouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	bool Is_Sprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	float Forward;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	float Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position", meta = (AllowPrivateAccess = "true"))
	float Player_Yaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position", meta = (AllowPrivateAccess = "true"))
	float Player_Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position", meta = (AllowPrivateAccess = "true"))
	bool MoveDirection;
	
};
