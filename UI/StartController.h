// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StartController.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API AStartController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UAwaitHUD* AwaitHUDWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UMainStartUI* MainStartHUDWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UMainLoadingHUD* MainLoadingHUDWidget;

	void Mouse();
	bool Start = false;

	float MouseX;
	float MouseY;
	float MouseXX;
	float MouseYY;
	float TimeSet = 0.0f;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void SetupInput();
};
