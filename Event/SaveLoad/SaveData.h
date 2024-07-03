// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveData.generated.h"

/**
 * All members you want to serialize must use UPROPERTY() macro
 */
USTRUCT()
struct SPECTER_API FSaveData
{
	GENERATED_BODY()
public:
	//Serializable members 
	UPROPERTY()
	FString ActorLabel;
	UPROPERTY()
	FVector Location;
	UPROPERTY()
	int var;
	UPROPERTY()
	bool Flag;
	UPROPERTY()
	FString Str;

	////Object initializer instead of constructor
	//void Init(class ASaveTestActor* Actor);
	FSaveData();
	/*FSaveData(class ASaveTestActor* Actor);*/
	//Serialize members to json string and return
	FString JsonSerialize();
	//Deserialize the object passed by argument and set the datas to actor
	//void SetDeserializedData(const TSharedPtr<FJsonObject>& JsonObject, ASaveTestActor* Actor);
private:
	//Deserialize and set simple data to struct
	void JsonDeserialize(const TSharedPtr<FJsonObject>& JsonObject);
	//Deserialize and set complex data to struct. In this case, FVector type is it.
	void JsonDeserializeLocation(const TSharedPtr<FJsonObject>& JsonObject);
};
