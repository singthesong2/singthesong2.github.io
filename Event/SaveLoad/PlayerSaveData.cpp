// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSaveData.h"
#include "../../WeaponComp/GunBase.h"
#include "UObject/ConstructorHelpers.h"
#include "JsonUtilities.h"

FPlayerSaveData::FPlayerSaveData()
{
	//this->ActorLabel = TEXT("");
	this->Location = FVector::Zero();
}

FPlayerSaveData::FPlayerSaveData(AMyCharacter* Actor)
{
	this->ActorTag = Actor->Tags.IsEmpty() ? FString("") : Actor->Tags[0].ToString();
	this->Location = Actor->GetSavedLocation();
	this->Rotation = Actor->GetActorRotation();
	this->AmmoSlot1 = Actor->GetWeaponComponent()->Ammo[0];
	this->AmmoSlot2 = Actor->GetWeaponComponent()->Ammo[1];
	this->AmmoSlot3 = Actor->GetWeaponComponent()->Ammo[2];

	if (IsValid(Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot1)]))
	{
		this->GunSlot1 = static_cast<int>(Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot1)]->WeaponLoadType);
		this->GunSlot1Clip = Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot1)]->GetCurrClip();
	}
	else
	{
		this->GunSlot1 = static_cast<int>(EWeaponLoadType::End);
		this->GunSlot1Clip = 0;
	}

	if (IsValid(Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot2)]))
	{
		this->GunSlot2 = static_cast<int>(Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot2)]->WeaponLoadType);
		this->GunSlot2Clip = Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot2)]->GetCurrClip();
	}
	else
	{
		this->GunSlot2 = static_cast<int>(EWeaponLoadType::End);
		this->GunSlot2Clip = 0;
	}

	if (IsValid(Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot3)]))
	{
		this->GunSlot3 = static_cast<int>(Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot3)]->WeaponLoadType);
		this->GunSlot3Clip = Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot3)]->GetCurrClip();
	}
	else
	{
		this->GunSlot3 = static_cast<int>(EWeaponLoadType::End);
		this->GunSlot3Clip = 0;
	}
	this->NightVisionState = Actor->GetNightVisionState();
}

//Serialize members to json string and return
FString FPlayerSaveData::JsonSerialize()
{
	FString str = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(FPlayerSaveData::StaticStruct(), this, str, 0, 0);
	//UE_LOG(LogTemp, Log, TEXT("Serialized : %s"), *str);
	return str;
}

//Deserialize the object passed by argument and set the datas to actor
void FPlayerSaveData::SetDeserializedData(const TSharedPtr<FJsonObject>& JsonObject, AMyCharacter* Actor)
{
	JsonDeserialize(JsonObject);
	//UE_LOG(LogTemp, Log, TEXT("ActorLabel : %s"), *ActorLabel);

	Actor->SetActorLocation(this->Location);
	Actor->SetActorRotation(this->Rotation);

	TArray<EWeaponLoadType> CreateArray;

	if (GunSlot1 < static_cast<int>(EWeaponLoadType::End))
	{
		CreateArray.Add(static_cast<EWeaponLoadType>(GunSlot1));
	}
	if (GunSlot2 < static_cast<int>(EWeaponLoadType::End))
	{
		CreateArray.Add(static_cast<EWeaponLoadType>(GunSlot2));
	}
	if (GunSlot3 < static_cast<int>(EWeaponLoadType::End))
	{
		CreateArray.Add(static_cast<EWeaponLoadType>(GunSlot3));
	}

	Actor->GetWeaponComponent()->LoadWeapons(CreateArray);

	AGunBase* Gun1 = Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot1)];
	AGunBase* Gun2 = Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot2)];
	AGunBase* Gun3 = Actor->GetWeaponComponent()->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot3)];
	
	if (IsValid(Gun1))
	{
		Gun1->SetCurrClip(GunSlot1Clip);
	}

	if (IsValid(Gun2))
	{
		Gun2->SetCurrClip(GunSlot2Clip);
	}

	if (IsValid(Gun3))
	{
		Gun3->SetCurrClip(GunSlot3Clip);
	}

	Actor->GetWeaponComponent()->Ammo[0] = AmmoSlot1;
	Actor->GetWeaponComponent()->Ammo[1] = AmmoSlot2;
	Actor->GetWeaponComponent()->Ammo[2] = AmmoSlot3;
}

//Deserialize and set simple data to struct
void FPlayerSaveData::JsonDeserialize(const TSharedPtr<FJsonObject>& JsonObject)
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
		else if (FString("Rotation") == FieldName)
		{
			JsonDeserializeRotation(Field.Value->AsObject());
			continue;
		}
		else if (FString("GunSlot1") == FieldName)
		{
			JsonObject->TryGetNumberField(Field.Key, GunSlot1);
		}
		else if (FString("GunSlot2") == FieldName)
		{
			JsonObject->TryGetNumberField(Field.Key, GunSlot2);
		}
		else if (FString("GunSlot3") == FieldName)
		{
			JsonObject->TryGetNumberField(Field.Key, GunSlot3);
		}
		else if (FString("AmmoSlot1") == FieldName)
		{
			JsonObject->TryGetNumberField(Field.Key, AmmoSlot1);
		}
		else if (FString("AmmoSlot2") == FieldName)
		{
			JsonObject->TryGetNumberField(Field.Key, AmmoSlot2);
		}
		else if (FString("AmmoSlot3") == FieldName)
		{
			JsonObject->TryGetNumberField(Field.Key, AmmoSlot3);
		}
		else if (FString("GunSlot1Clip") == FieldName)
		{
			JsonObject->TryGetNumberField(Field.Key, GunSlot1Clip);
		}
		else if (FString("GunSlot2Clip") == FieldName)
		{
			JsonObject->TryGetNumberField(Field.Key, GunSlot2Clip);
		}
		else if (FString("GunSlot3Clip") == FieldName)
		{
			JsonObject->TryGetNumberField(Field.Key, GunSlot3Clip);
		}
		else if (FString("NightVisionState") == FieldName)
		{
			JsonObject->TryGetBoolField(Field.Key, NightVisionState);
		}
		else if (FString("ActorTag") == FieldName)
		{
			JsonObject->TryGetStringField(Field.Key, ActorTag);
		}
	}
}

//Deserialize and set complex data to struct. In this case, FVector type is it.
void FPlayerSaveData::JsonDeserializeLocation(const TSharedPtr<FJsonObject>& JsonObject)
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

void FPlayerSaveData::JsonDeserializeRotation(const TSharedPtr<FJsonObject>& JsonObject)
{
	TArray<FString> Keys = TArray<FString>();
	for (auto& Field : JsonObject->Values)
	{
		Keys.Add(Field.Key);
	}
	JsonObject->TryGetNumberField(Keys[0], Rotation.Pitch);
	JsonObject->TryGetNumberField(Keys[1], Rotation.Yaw);
	JsonObject->TryGetNumberField(Keys[2], Rotation.Roll);
}