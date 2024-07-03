// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "MyCharacter_Controller.generated.h"

UCLASS()
class SPECTER_API AMyCharacter_Controller : public APlayerController, public IGenericTeamAgentInterface {
	GENERATED_BODY()

public:
	AMyCharacter_Controller();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UPauseUI* PauseHUDWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UStopPauseUI* StopPauseHUDWidget;

	// AI와 같은 팀일지 여부입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool team = false;
	
private: 
	FGenericTeamId TeamId = FGenericTeamId(5);	// Implement The Generic Team Interface
	virtual FGenericTeamId GetGenericTeamId() const override;

public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
};
