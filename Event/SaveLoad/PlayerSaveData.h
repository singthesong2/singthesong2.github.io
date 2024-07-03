// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Character/MyCharacter.h"
#include "../../WeaponComp/WeaponComponent.h"
#include "../../WeaponComp/GunBase.h"
#include "PlayerSaveData.generated.h"

/**
 * All members you want to serialize must use UPROPERTY() macro
 */
USTRUCT()
struct SPECTER_API FPlayerSaveData
{
	GENERATED_BODY()
public:
	//Serializable members 
	UPROPERTY()
	FString ActorTag;
	UPROPERTY()
	FVector Location;
	UPROPERTY()
	FRotator Rotation;
public:
	UPROPERTY()
	int GunSlot1;

	UPROPERTY()
	int GunSlot2;

	UPROPERTY()
	int GunSlot3;

	UPROPERTY()
	int AmmoSlot1;

	UPROPERTY()
	int AmmoSlot2;

	UPROPERTY()
	int AmmoSlot3;

	UPROPERTY()
	int GunSlot1Clip;

	UPROPERTY()
	int GunSlot2Clip;

	UPROPERTY()
	int GunSlot3Clip;

	UPROPERTY()
	bool NightVisionState;
	////Object initializer instead of constructor
	//void Init(class ASaveTestActor* Actor);
	FPlayerSaveData();
	FPlayerSaveData(class AMyCharacter* Actor);

	//Serialize members to json string and return
	FString JsonSerialize();
	//Deserialize the object passed by argument and set the datas to actor
	void SetDeserializedData(const TSharedPtr<FJsonObject>& JsonObject, AMyCharacter* Actor);
private:
	//Deserialize and set simple data to struct
	void JsonDeserialize(const TSharedPtr<FJsonObject>& JsonObject);
	//Deserialize and set complex data to struct. In this case, FVector type is it.
	void JsonDeserializeLocation(const TSharedPtr<FJsonObject>& JsonObject);
	void JsonDeserializeRotation(const TSharedPtr<FJsonObject>& JsonObject);
};