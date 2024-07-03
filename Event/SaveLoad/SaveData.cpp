// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveData.h"
//#include "SaveTestActor.h"
#include "JsonUtilities.h"

//void FSaveData::Init(ASaveTestActor* Actor)
//{
//	this->ActorLabel = Actor->GetActorLabel();
//	this->Location = Actor->GetActorLocation();
//	this->var = Actor->var;
//	this->Flag = Actor->flag;
//	this->Str = Actor->Str;	
//}

FSaveData::FSaveData()
{
	this->ActorLabel = TEXT("");
	this->Location = FVector::Zero();
	this->var = 0;
	this->Flag = false;
	this->Str = TEXT("");
}

//FSaveData::FSaveData(ASaveTestActor* Actor)
//{
//	this->ActorLabel = Actor->GetActorLabel();
//	this->Location = Actor->GetActorLocation();
//	this->var = Actor->var;
//	this->Flag = Actor->flag;
//	this->Str = Actor->Str;
//}

//Serialize members to json string and return
FString FSaveData::JsonSerialize()
{
	FString str = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(FSaveData::StaticStruct(), this, str, 0, 0);
	//UE_LOG(LogTemp, Log, TEXT("Serialized : %s"), *str);
	return str;
}

//Deserialize the object passed by argument and set the datas to actor
//void FSaveData::SetDeserializedData(const TSharedPtr<FJsonObject>& JsonObject, ASaveTestActor* Actor)
//{
//	JsonDeserialize(JsonObject);
//	UE_LOG(LogTemp, Log, TEXT("ActorLabel : %s, var: %d, Str: %s, Flag = %d"), *ActorLabel, var, *Str, Flag == true ? 1 : 0);
//
//	Actor->SetActorLocation(this->Location);
//	Actor->var = this->var;
//	Actor->flag = this->Flag;
//	Actor->Str = this->Str;
//}

//Deserialize and set simple data to struct
void FSaveData::JsonDeserialize(const TSharedPtr<FJsonObject>& JsonObject)
{
	//Json key string array
	TArray<FString> Keys = TArray<FString>();

	for (auto& Field : JsonObject->Values)
	{
		//FieldName is member variable name
		FString FieldName = Field.Key;

		//Exceptional deserialization for object type member
		if ("Location" == FieldName)
		{
			//Deserialize one more time with selected member object
			JsonDeserializeLocation(Field.Value->AsObject());
			continue;
		}
		//Set found key to array
		Keys.Add(Field.Key);
	}

	//Extract values from json object by each key and set them to correct variable
	JsonObject->TryGetStringField(Keys[0], ActorLabel);
	JsonObject->TryGetNumberField(Keys[1], var);
	JsonObject->TryGetBoolField(Keys[2], Flag);
	JsonObject->TryGetStringField(Keys[3], Str);

}

//Deserialize and set complex data to struct. In this case, FVector type is it.
void FSaveData::JsonDeserializeLocation(const TSharedPtr<FJsonObject>& JsonObject)
{
	TArray<FString> Keys = TArray<FString>();
	for (auto& Field : JsonObject->Values)
	{
		Keys.Add(Field.Key);
	}
	JsonObject->TryGetNumberField(Keys[0], Location.X);
	JsonObject->TryGetNumberField(Keys[1], Location.Y);
	JsonObject->TryGetNumberField(Keys[2], Location.Z);
}
