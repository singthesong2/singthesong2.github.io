// Fill out your copyright notice in the Description page of Project Settings.


#include "StartController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/MainLoadingHUD.h"
#include "UI/MainStartUI.h"
#include "UI/AwaitHUD.h"
#include "UI/StartGameMode.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "InputCoreTypes.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

void AStartController::BeginPlay()
{
	Super::BeginPlay();

	AStartGameMode* GameMode = Cast<AStartGameMode>(GetWorld()->GetAuthGameMode());
	AwaitHUDWidget = GameMode->GetAwaitHUDWidget();
	MainStartHUDWidget = GameMode->GetMainStartHUDWidget();
	MainLoadingHUDWidget = GameMode->GetMainLoadingHUDWidget();

	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableTouchEvents = false;

	SetupInput();
}

void AStartController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	if (Start == true)
	{

		TimeSet += DeltaTime;
		if (TimeSet < 0.1f)
		{
			MouseX = MousePosition.X;
			MouseY = MousePosition.Y;
		}
	
		if (TimeSet >= 3.0f)
		{
			MouseXX = MousePosition.X;
			MouseYY = MousePosition.Y;

			if (MouseX == MouseXX && MouseY == MouseYY)
			{
				AwaitHUDWidget->HUDOn();
				MainStartHUDWidget->BackMusicOff();
			}
			else
			{
				AwaitHUDWidget->HUDOff();
				MainStartHUDWidget->BackMusicOn();
				TimeSet = 0.0f;
			}
		}
	}
}

void AStartController::Mouse()
{
	Start = true;
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	MainLoadingHUDWidget->RemoveFromParent();
}

void AStartController::SetupInput()
{
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	InputComponent->BindAction("AnyKey", IE_Pressed, this, &AStartController::Mouse);
}