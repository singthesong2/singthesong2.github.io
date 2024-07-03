// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/*
UENUM(BlueprintType)
enum class E_ParkourMovementType : uint8 {
	E_None				UMETA(DisplayName="None"),
	E_LeftWallRun		UMETA(DisplayName="LeftWallRun"),
	E_RightWallRun		UMETA(DisplayName="RightWallRun"),
	E_VerticalWallRun	UMETA(DisplayName="VerticalWallRun"),
	E_LedgeGrab			UMETA(DisplayName="LedgeGrab"),
	E_Mantle			UMETA(DisplayName="Mantle"),
	E_Slide				UMETA(DisplayName="Slide"),
	E_Crouch			UMETA(DisplayName="Crouch"),
	E_Sprint			UMETA(DisplayName="Sprint")
};
*/

UENUM(BlueprintType)
enum class E_EnemyState : uint8 {
	E_StateOff		UMETA(DisplayName="없음 Off"),
	E_StatePatrol	UMETA(DisplayName="순찰 Patrol"),
	E_StateAlert	UMETA(DisplayName="수색 Alert"),
	E_StateSearch	UMETA(DisplayName="탐지 Search"),
	E_StateAttack	UMETA(DisplayName="공격 Attack"),
	E_StateComeback	UMETA(DisplayName="복귀 Comeback"),
	E_StateDead		UMETA(DisplayName="사망 Dead")
};

UENUM(BlueprintType)
enum class E_EnemyPatrolType : uint8 {
	Patrol_Point	UMETA(DisplayName="지점 Point"),
	Patrol_Spline	UMETA(DisplayName="스플라인 Spline"),
	Patrol_Random	UMETA(DisplayName="무작위 Random"),
	Patrol_Stay		UMETA(DisplayName="고정 Stay")
};

UENUM(BlueprintType)
enum class E_EnemySearchType : uint8 {
	Search_Normal	UMETA(DisplayName="Normal"),
	Search_Infinity	UMETA(DisplayName="Infinity")
};

UENUM(BlueprintType)
enum class E_EnemyCombatType : uint8 {
	Combat_None			UMETA(DisplayName = "없음 None"),
	Combat_Close		UMETA(DisplayName = "접근형 Close"),
	Combat_Defence		UMETA(DisplayName = "수비형 Defence"),
	Combat_Neutral		UMETA(DisplayName = "비공격형 Neutral")        
};

UENUM(BlueprintType)
enum class E_EnemyCombatCoverType : uint8 {
	Cover_None		UMETA(DisplayName="없음 None"),
	Cover_Crouch	UMETA(DisplayName="숙이기 Crouch"),
	Cover_WallHide	UMETA(DisplayName="벽뒤 WallHide")
};

/*
UENUM(BlueprintType)
enum class E_EnemyAlertType : uint8 {
	E_FindEnemy			UMETA(DisplayName="FindEnemy"),
	E_LostEnemy			UMETA(DisplayName="LostEnemy"),
	E_FoundDeadAlly		UMETA(DisplayName="FoundDeadAlly"),
	E_EnemyIsDead		UMETA(DisplayName="EnemyDead")
};
*/