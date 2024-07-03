// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/StageLoadingHUD.h"
#include "Kismet/GameplayStatics.h"

void UStageLoadingHUD::NativeConstruct()
{
	Super::NativeConstruct();

	StageBackLoad = Cast<UImage>(GetWidgetFromName(TEXT("StageBackLoad")));
	MapImage= Cast<UImage>(GetWidgetFromName(TEXT("MapImage")));
	StageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("StageText")));
	StageName = Cast<UTextBlock>(GetWidgetFromName(TEXT("StageName")));
}

UStageLoadingHUD::UStageLoadingHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> MapObj(TEXT("/Game/UI/Sprite/Stage1_Title.Stage1_Title"));

	static ConstructorHelpers::FObjectFinder<UTexture2D> MapObj2(TEXT("/Game/UI/Sprite/Stage2_Title.Stage2_Title"));

	static ConstructorHelpers::FObjectFinder<UTexture2D> MapObj3(TEXT("/Game/UI/Sprite/Stage3_Title.Stage3_Title"));

	if (MapObj.Succeeded())
	{
		MapText1 = Cast<UTexture2D>(MapObj.Object);
	}

	if (MapObj2.Succeeded())
	{
		MapText2 = Cast<UTexture2D>(MapObj2.Object);
	}

	if (MapObj3.Succeeded())
	{
		MapText3 = Cast<UTexture2D>(MapObj3.Object);
	}
}

void UStageLoadingHUD::StageLoadOn()
{
	// Stage 1
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Demo_P"))
	{
		MapImage->SetBrushFromTexture(MapText1, false);
		FName Text;
		FName Name;
		Name = FName(TEXT("잠입"));
		Text = FName(TEXT("깊은 산골에 있는 데저트 울프의 본기지. 심상치 않은 모의를 하고 있는 것 같다.\n은밀하게 침투하여 조사하라."));
		StageName->SetText(FText::FromName(Name));
		StageText->SetText(FText::FromName(Text));
	}
	 //Stage 2
	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Demo_P_Level02_Main"))
	{
		MapImage->SetBrushFromTexture(MapText2, false);
		FName Text;
		FName Name;
		Name = FName(TEXT("해킹"));
		Text = FName(TEXT("데저트 울프의 본기지에 무사히 잠입했고,그 곳에는 거대한 미사일이 배치되어 있다.\n해킹을 통해 음모를 파악하라."));
		StageName->SetText(FText::FromName(Name));
		StageText->SetText(FText::FromName(Text));
	}
	 //Stage 3
	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Demo_P_Level03_Main"))
	{
		MapImage->SetBrushFromTexture(MapText3, false);
		FName Text;
		FName Name;
		Name = FName(TEXT("탈출"));
		Text = FName(TEXT("미사일을 테러 조직에 판매하려는 음모는 파악했지만, 지원을 요청할 시간은 없다.\n기차가 거래 장소에 도착하기 전에, 카드키를 탈취하여 음모를 저지하라."));
		StageName->SetText(FText::FromName(Name));
		StageText->SetText(FText::FromName(Text));
	}

	StageBackLoad->SetVisibility(ESlateVisibility::Visible);
	MapImage->SetVisibility(ESlateVisibility::Visible);
	StageText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UStageLoadingHUD::StageLoadOff, 4.0f, false);
}

void UStageLoadingHUD::StageLoadOff()
{
	RemoveFromParent();
}