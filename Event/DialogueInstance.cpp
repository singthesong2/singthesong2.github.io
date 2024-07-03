// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueInstance.h"
#include "DialogueTable.h"
#include "Saveload/SavableInterface.h"
#include "SaveLoad/SaveSystem.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/MyCharacter.h"

UDialogueInstance::UDialogueInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>DataTable(TEXT("'/Game/Event/Dialogue/InteractionTable.InteractionTable'"));
	if (false == DataTable.Succeeded()) 
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to load dialogue data table"));
		return;
	}
	DialogueTable = DataTable.Object;
}

void UDialogueInstance::ResetActors()
{
	Actors.Empty();
}

void UDialogueInstance::Init()
{
	Super::Init();
}

FDialogueData* UDialogueInstance::GetDialogueData(int Row)
{
	return DialogueTable->FindRow<FDialogueData>(*FString::FromInt(Row), TEXT(""));
}

FDialogueData* UDialogueInstance::GetDialogueData(FString Row)
{
	return DialogueTable->FindRow<FDialogueData>(FName(*Row), TEXT(""));
}

void UDialogueInstance::SetActorToList(AActor* Actor)
{
	if (!Actor->GetClass()->ImplementsInterface(USavableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("Wrong actor is try to add to savable list %s"), *Actor->GetName());
		return;
	}
	Actors.Add(Actor);
	UE_LOG(LogTemp, Log, TEXT("Actor added, current count = %d"), Actors.Num());
}

void UDialogueInstance::SaveActorToJson(const FVector& SaveLocation)
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

void UDialogueInstance::DeleteSaveData()
{
	USaveSystem* Save = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(USaveSystem::StaticClass()));
	if (nullptr == Save) 
	{
		return;
	}
	UGameplayStatics::DeleteGameInSlot(SlotName, Index);
}

void UDialogueInstance::LoadJson()
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
		FLatentActionInfo LActionInfo;
		UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Project_Test_Level"), true, true, LActionInfo);
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Project_Test_Level"));
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
}

TSharedPtr<FJsonObject> UDialogueInstance::FindJsonByTag(const FName& Tag)
{
	FString _Tag;
	UE_LOG(LogTemp, Log, TEXT("JObj count = %d"), JsonObjects.Num());
	for (auto& jObj : JsonObjects)
	{
		if (false == jObj->TryGetStringField(TEXT("ActorTag"), _Tag))
		{
			UE_LOG(LogTemp, Log, TEXT("Failed to get actor name from json object"));
			return nullptr;
		}
		if (FName(_Tag) == Tag)
		{
			UE_LOG(LogTemp, Log, TEXT("Found actor data with tag : %s"), *_Tag);
			return jObj;
		}
	}
	return nullptr;
}
