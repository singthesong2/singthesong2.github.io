// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "DialogueInstance.generated.h"

/**
 *
 */
UCLASS()
class SPECTER_API UDialogueInstance : public UGameInstance
{
	GENERATED_BODY()
	TArray<AActor*> Actors;
	TArray<TSharedPtr<FJsonObject>> JsonObjects;	
	FString SlotName = TEXT("Save");
	int Index = 1;
public:
	UDialogueInstance();
	void ResetActors();
	virtual void Init() override;
	struct FDialogueData* GetDialogueData(int Row);
	struct FDialogueData* GetDialogueData(FString Row);
	void SetActorToList(AActor* Actor);
	void SaveActorToJson(const FVector& SaveLocation);
	void DeleteSaveData();
	void LoadJson();
	TSharedPtr<FJsonObject> FindJsonByTag(const FName& Tag);
private:
	UPROPERTY()
	class UDataTable* DialogueTable;
};
