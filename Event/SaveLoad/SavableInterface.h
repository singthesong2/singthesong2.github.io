#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Templates/SharedPointer.h"
#include "SavableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class USavableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class SPECTER_API ISavableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetActorToSavableList();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FString JsonSerialize();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool JsonDeserialize();
};
