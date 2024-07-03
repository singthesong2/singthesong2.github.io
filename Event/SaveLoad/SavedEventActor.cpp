// Fill out your copyright notice in the Description page of Project Settings.

#include "Event/SaveLoad/SavedEventActor.h"
#include "../EventActor.h"
#include "JsonUtilities.h"

FSavedEventActor::FSavedEventActor()
{

}

FSavedEventActor::FSavedEventActor(AEventActor* Actor)
{
	ActorTag = Actor->Tags.IsEmpty() ? FString("") : Actor->Tags[0].ToString();
	IsCompletedEvent = Actor->IsCompletedEvent;
	IsOngoingEvent = Actor->IsOngoingEvent;
	IsMarkerActived = Actor->IsMarkerActived;
	IsOnActive = Actor->IsOnActive;
}

FString FSavedEventActor::JsonSerialize()
{
	FString str = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(FSavedEventActor::StaticStruct(), this, str, 0, 0);
	return str;
}

void FSavedEventActor::SetDeserializedData(const TSharedPtr<FJsonObject>& JsonObject, AEventActor* Actor)
{
	JsonDeserialize(JsonObject);

	Actor->IsCompletedEvent = this->IsCompletedEvent;

	Actor->IsOngoingEvent = this->IsOngoingEvent;
	Actor->IsMarkerActived = this->IsMarkerActived;
	Actor->IsOnActive = this->IsOnActive;
}

void FSavedEventActor::JsonDeserialize(const TSharedPtr<FJsonObject>& JsonObject)
{
	//Json key string array
	TArray<FString> Keys = TArray<FString>();

	for (auto& Field : JsonObject->Values)
	{
		//FieldName is member variable name
		FString FieldName = Field.Key;

		//Exceptional deserialization for object type member
		//if ("EventFlags" == FieldName)
		//{
		//	//Deserialize one more time with selected member object
		//	JsonDeserializeFlags(Field.Value->AsObject());
		//	continue;
		//}
		//Set found key to array
		Keys.Add(Field.Key);
	}
	int index = 0;
	//Extract values from json object by each key and set them to correct variable
	JsonObject->TryGetStringField(Keys[index++], ActorTag);
	JsonObject->TryGetBoolField(Keys[index++], IsCompletedEvent);
	JsonObject->TryGetBoolField(Keys[index++], IsOngoingEvent);
	JsonObject->TryGetBoolField(Keys[index++], IsMarkerActived);
	JsonObject->TryGetBoolField(Keys[index++], IsOnActive);

}

void FSavedEventActor::JsonDeserializeFlags(const TSharedPtr<FJsonObject>& JsonObject)
{
	//TArray<FString> Keys = TArray<FString>();
	//for (auto& Field : JsonObject->Values)
	//{
	//	Keys.Add(Field.Key);
	//	EventFlags.Add(false);
	//}
	//for (int i = 0; i < Keys.Num(); i++)
	//{
	//	JsonObject->TryGetBoolField(Keys[i], EventFlags[i]);
	//}
}
