// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SavedEventActor.generated.h"

/**
 * 
 */
USTRUCT()
struct SPECTER_API FSavedEventActor
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	FString ActorTag;
	UPROPERTY()
	bool IsCompletedEvent;
	UPROPERTY()
	bool IsOngoingEvent;
	UPROPERTY()
	bool IsMarkerActived;
	UPROPERTY()
	bool IsOnActive;
public:
	FSavedEventActor();
	FSavedEventActor(class AEventActor* Actor);
	FString JsonSerialize(); 
	void SetDeserializedData(const TSharedPtr<FJsonObject>& JsonObject, AEventActor* Actor);
private:
	void JsonDeserialize(const TSharedPtr<FJsonObject>& JsonObject);
	//Deserialize and set complex data to struct. In this case, FVector type is it.
	void JsonDeserializeFlags(const TSharedPtr<FJsonObject>& JsonObject);
};
