// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AwaitHUD.h"

UAwaitHUD::UAwaitHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMediaSource> Media(TEXT("/Game/UI/SPECTER_HUD_Trailer.SPECTER_HUD_Trailer"));
	if (Media.Succeeded())
	{
		Video = Cast<UMediaSource>(Media.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMediaPlayer> Player(TEXT("/Game/UI/TestMedia.TestMedia"));
	if (Player.Succeeded())
	{
		MediaPlayer = Cast<UMediaPlayer>(Player.Object);
	}

}

void UAwaitHUD::NativeConstruct()
{
	Super::NativeConstruct();
	MediaPlayer->OpenSource(Video);
	AwaitBack = Cast<UImage>(GetWidgetFromName(TEXT("AwaitBack")));
	AwaitText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AwaitText")));
}

void UAwaitHUD::HUDOn()
{
	AwaitBack->SetVisibility(ESlateVisibility::Visible);
}
void UAwaitHUD::HUDOff()
{
	AwaitBack->SetVisibility(ESlateVisibility::Hidden);
}