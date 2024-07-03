// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameMode.h"

AStartGameMode::AStartGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Script/Specter.StartController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UMainStartUI> MainStartHUDWidgetAsset(TEXT("/Game/UI/WBPStartUI.WBPStartUI_C"));

	if (MainStartHUDWidgetAsset.Succeeded())
	{
		MainStartHUDWidgetClass = MainStartHUDWidgetAsset.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UMainLoadingHUD> MainLoadingHUDWidgetAsset(TEXT("/Game/UI/WBPMainLoading.WBPMainLoading_C"));

	if (MainLoadingHUDWidgetAsset.Succeeded())
	{
		MainLoadingHUDWidgetClass = MainLoadingHUDWidgetAsset.Class;
	}

	static ConstructorHelpers::FClassFinder<UAwaitHUD>AwaitHUDWidgetAsset(TEXT("/Game/UI/WBP_AwaitUI.WBP_AwaitUI_C"));
	
	if (AwaitHUDWidgetAsset.Succeeded())
	{
		AwaitHUDWidgetClass = AwaitHUDWidgetAsset.Class;
	}
}

void AStartGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(MainStartHUDWidgetClass))
	{
		MainStartHUDWidget = Cast<UMainStartUI>(CreateWidget(GetWorld(), MainStartHUDWidgetClass));

		if (IsValid(MainStartHUDWidget))
			MainStartHUDWidget->AddToViewport();
	}

	if (IsValid(MainLoadingHUDWidgetClass))
	{
		MainLoadingHUDWidget = Cast<UMainLoadingHUD>(CreateWidget(GetWorld(), MainLoadingHUDWidgetClass));

		if (IsValid(MainLoadingHUDWidget))
		{
			MainLoadingHUDWidget->AddToViewport();
			//GetWorld()->GetTimerManager().SetTimer(timer, this, &AStartGameMode::LoadingControll, 4.0f, false);
		}
	}

	if (IsValid(AwaitHUDWidgetClass))
	{
		AwaitHUDWidget = Cast<UAwaitHUD>(CreateWidget(GetWorld(), AwaitHUDWidgetClass));

		if (IsValid(AwaitHUDWidget))
			AwaitHUDWidget->AddToViewport();
	}
}

void AStartGameMode::LoadingControll()
{
	AStartController* Controller = Cast<AStartController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	MainLoadingHUDWidget->RemoveFromParent();

	if (IsValid(Controller))
	{
		Controller->Mouse();
	}
}

