// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveData.h"
/*
void UMyGameInstance::SetActorToList(AActor* Actor)
{
	if (!Actor->GetClass()->ImplementsInterface(USavableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("Wrong actor is try to add to savable list %s"), *Actor->GetName());
		return;
	}
	Actors.Add(Actor);
	UE_LOG(LogTemp, Log, TEXT("Actor added, current count = %d"), Actors.Num());
}

void UMyGameInstance::SaveActorToJson()
{
	FString jStr = TEXT("");
	USaveSystem* Save = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(USaveSystem::StaticClass()));
	if (nullptr == Save)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to generate save data"));
		return;
	}
	jStr += TEXT("{\"Datas\": [");
	for (auto actor : Actors)
	{
		if (actor->GetClass()->ImplementsInterface(USavableInterface::StaticClass()))
		{
			jStr += ISavableInterface::Execute_JsonSerialize(actor);
			jStr += TEXT(",");
		}
	}
	jStr.RemoveFromEnd(TEXT(","));
	jStr += TEXT("]}");
	UE_LOG(LogTemp, Log, TEXT("Final Json string = %s"), *jStr);
	Save->JsonString = jStr;
	UGameplayStatics::SaveGameToSlot(Save, SlotName, Index);
}

void UMyGameInstance::LoadJson()
{
	USaveSystem* Save = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(USaveSystem::StaticClass()));
	if (nullptr == Save)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to generate save data"));
		return;
	}
	Save = Cast<USaveSystem>(UGameplayStatics::LoadGameFromSlot(SlotName, Index));	
	if (nullptr == Save)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to load json"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Loaded json string = %s"), *Save->JsonString);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Save->JsonString);
	TSharedPtr<FJsonObject> JObj = MakeShareable(new FJsonObject());

	if (false == FJsonSerializer::Deserialize(JsonReader, JObj))
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to deserialize loaded json"));
		return;
	}
	const TArray<TSharedPtr<FJsonValue>>* DataArray;
	if (false == JObj->TryGetArrayField(TEXT("Datas"), DataArray))
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to get field from string"));
		return;
	}
	for (const TSharedPtr<FJsonValue>& Value : *DataArray)
	{
		if (Value->Type == EJson::Object)
		{
			TSharedPtr<FJsonObject> Obj = Value->AsObject();
			JsonObjects.Add(Obj);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Deserialized, count = %d"), JsonObjects.Num());
	//for (int i = 0; i < 3; i++)
	//{
	//	if (!Actors[i]->GetClass()->ImplementsInterface(USavable::StaticClass()))
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("Wrong actor is try to add to savable list %s"), *Actors[i]->GetName());
	//		return;
	//	}
	//	ISavable::Execute_JsonDeserialize(Actors[0]);
	//}

	UE_LOG(LogTemp, Log, TEXT("Actors count = %d"), Actors.Num());
	for (auto& actor : Actors)
	{
		if (false == actor->GetClass()->ImplementsInterface(USavableInterface::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("Wrong actor is try to add to savable list %s"), *actor->GetActorLabel());
			continue;
		}
		ISavableInterface::Execute_JsonDeserialize(actor);
	}
}

TSharedPtr<FJsonObject> UMyGameInstance::FindJsonByLabel(const FString& Label)
{
	FString actorName;
	UE_LOG(LogTemp, Log, TEXT("JObj count = %d"), JsonObjects.Num());
	for (auto& jObj : JsonObjects)
	{
		if (false == jObj->TryGetStringField(TEXT("ActorLabel"), actorName))
		{
			UE_LOG(LogTemp, Log, TEXT("Failed to get actor name from json object"));
			return nullptr;
		}
		if (actorName == Label)
		{
			UE_LOG(LogTemp, Log, TEXT("Found %s actor data"), *Label);
			return jObj;
		}
	}
	return nullptr;
}
*/