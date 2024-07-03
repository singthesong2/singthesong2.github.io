// Fill out your copyright notice in the Description page of Project Settings.


#include "MainStartUI.h"
#include "DialogueInstance.h"

void UMainStartUI::NativeConstruct()
{
	Super::NativeConstruct();

	MainBackGround =Cast<UImage>(GetWidgetFromName(TEXT("MainBackGround")));
	MainTitleImage = Cast<UImage>(GetWidgetFromName(TEXT("TitleImage")));
	MainName =Cast<UTextBlock>(GetWidgetFromName(TEXT("MainName")));
	MainStart = Cast<UTextBlock>(GetWidgetFromName(TEXT("MainStart")));
	MainExit = Cast<UTextBlock>(GetWidgetFromName(TEXT("MainExit")));
	MainStartButton =Cast<UButton>(GetWidgetFromName(TEXT("MainStartButton")));
	MainExitButton = Cast<UButton>(GetWidgetFromName(TEXT("MainExitButton")));

	GameSelectButton = Cast<UButton>(GetWidgetFromName(TEXT("GameSelectButton")));
	GameSelect = Cast<UTextBlock>(GetWidgetFromName(TEXT("GameSelect")));
	Stage1Button=Cast<UButton>(GetWidgetFromName(TEXT("Stage1Button")));
	Stage2Button = Cast<UButton>(GetWidgetFromName(TEXT("Stage2Button")));
	Stage3Button = Cast<UButton>(GetWidgetFromName(TEXT("Stage3Button")));
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
	Stage1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Stage1")));
	Stage2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Stage2")));
	Stage3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Stage3")));
	Back = Cast<UTextBlock>(GetWidgetFromName(TEXT("Back")));
	StageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("StageText")));
	StageImage=Cast<UImage>(GetWidgetFromName(TEXT("StageImage")));
	MissionText = Cast<UTextBlock>(GetWidgetFromName(TEXT("StageMission")));


	MainStartButton->OnClicked.AddDynamic(this, &UMainStartUI::StartButtonCallback);
	MainExitButton->OnClicked.AddDynamic(this, &UMainStartUI::ExitButtonCallback);
	GameSelectButton->OnClicked.AddDynamic(this, &UMainStartUI::GameSelectButtonCallback);
	Stage1Button->OnClicked.AddDynamic(this, &UMainStartUI::Stage1ButtonCallback);
	Stage2Button->OnClicked.AddDynamic(this, &UMainStartUI::Stage2ButtonCallback);
	Stage3Button->OnClicked.AddDynamic(this, &UMainStartUI::Stage3ButtonCallback);
	BackButton->OnClicked.AddDynamic(this, &UMainStartUI::BackButtonCallback);
	Stage1Button->OnHovered.AddDynamic(this, &UMainStartUI::Stage1ButtonHovered);
	Stage2Button->OnHovered.AddDynamic(this, &UMainStartUI::Stage2ButtonHovered);
	Stage3Button->OnHovered.AddDynamic(this, &UMainStartUI::Stage3ButtonHovered);
	Stage1Button->OnUnhovered.AddDynamic(this, &UMainStartUI::Stage1ButtonOffHovered);
	Stage2Button->OnUnhovered.AddDynamic(this, &UMainStartUI::Stage2ButtonOffHovered);
	Stage3Button->OnUnhovered.AddDynamic(this, &UMainStartUI::Stage3ButtonOffHovered);

	AudioControll = UGameplayStatics::SpawnSound2D(this, BackgroundMusic);
	AudioControll->bAutoDestroy = false;
}

UMainStartUI::UMainStartUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	static ConstructorHelpers::FObjectFinder<UMaterial> MapMaterial1(TEXT("/Game/UI/Sprite/Stage1_Title_Mat.Stage1_Title_Mat"));

	if (MapMaterial1.Succeeded())
	{
		MapMat1 = Cast<UMaterial>(MapMaterial1.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MapMaterial2(TEXT("/Game/UI/Sprite/Stage2_Title_Mat.Stage2_Title_Mat"));

	if (MapMaterial2.Succeeded())
	{
		MapMat2 = Cast<UMaterialInstance>(MapMaterial2.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MapMaterial3(TEXT("/Game/UI/Sprite/Stage3_Title_Mat.Stage3_Title_Mat"));

	if (MapMaterial3.Succeeded())
	{
		MapMat3 = Cast<UMaterialInstance>(MapMaterial3.Object);
	}	
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MapMaterial4(TEXT("/Game/UI/Sprite/StageNone_Title_Mat.StageNone_Title_Mat"));

	if (MapMaterial4.Succeeded())
	{
		MapMat4 = Cast<UMaterialInstance>(MapMaterial4.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> Sound(TEXT("/Game/Sound/Sound_Background/EpicUnderscore/EpicUnderscoreAssets/Cues/AcrossNoMansLand.AcrossNoMansLand"));

	if (Sound.Succeeded())
	{
		BackgroundMusic = Cast<USoundCue>(Sound.Object);
	}
}

void UMainStartUI::StartButtonCallback()
{
	RemoveFromParent();
	// 레벨을 바꿔주는 함수
	DeleteSave();
	FLatentActionInfo LActionInfo;
	UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Demo_P"), true, true, LActionInfo);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo_P"));
}

void UMainStartUI::GameSelectButtonCallback()
{
	MainStartButton->SetVisibility(ESlateVisibility::Hidden);
	MainStart->SetVisibility(ESlateVisibility::Hidden);
	MainTitleImage->SetVisibility(ESlateVisibility::Hidden);
	StageImage->SetVisibility(ESlateVisibility::Visible);
	StageImage->SetBrushFromMaterial(MapMat4);
	//StageText->SetVisibility(ESlateVisibility::Visible);
	//logo->SetVisibility(ESlateVisibility::Hidden);
	GameSelectButton->SetVisibility(ESlateVisibility::Hidden);
	GameSelect->SetVisibility(ESlateVisibility::Hidden);
	MainExitButton->SetVisibility(ESlateVisibility::Hidden);
	MainExit->SetVisibility(ESlateVisibility::Hidden);
	Stage1Button->SetVisibility(ESlateVisibility::Visible);
	Stage1->SetVisibility(ESlateVisibility::Visible);
	Stage2Button->SetVisibility(ESlateVisibility::Visible);
	Stage2->SetVisibility(ESlateVisibility::Visible);
	Stage3Button->SetVisibility(ESlateVisibility::Visible);
	Stage3->SetVisibility(ESlateVisibility::Visible);
	BackButton->SetVisibility(ESlateVisibility::Visible);
	Back->SetVisibility(ESlateVisibility::Visible);
}

void UMainStartUI::ExitButtonCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UMainStartUI::Stage1ButtonCallback()
{
	RemoveFromParent();
	DeleteSave();
	FLatentActionInfo LActionInfo;
	UGameplayStatics::LoadStreamLevel(this, TEXT("Demo_P"), true, true, LActionInfo);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo_P"));
}

void UMainStartUI::Stage2ButtonCallback()
{
	RemoveFromParent();
	DeleteSave();
	FLatentActionInfo LActionInfo;
	UGameplayStatics::LoadStreamLevel(this, TEXT("Demo_P_Level02_Main"), true, true, LActionInfo);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo_P_Level02_Main"));
}

void UMainStartUI::Stage3ButtonCallback()
{
	RemoveFromParent();
	DeleteSave();
	FLatentActionInfo LActionInfo;
	UGameplayStatics::LoadStreamLevel(this, TEXT("Demo_P_Level03_Main"), true, true, LActionInfo);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo_P_Level03_Main"));
}

void UMainStartUI::BackButtonCallback()
{
	MainStartButton->SetVisibility(ESlateVisibility::Visible);
	MainStart->SetVisibility(ESlateVisibility::Visible);
	MainTitleImage->SetVisibility(ESlateVisibility::Visible);
	GameSelectButton->SetVisibility(ESlateVisibility::Visible);
	GameSelect->SetVisibility(ESlateVisibility::Visible);
	MainExitButton->SetVisibility(ESlateVisibility::Visible);
	MainExit->SetVisibility(ESlateVisibility::Visible);
	StageImage->SetVisibility(ESlateVisibility::Hidden);
	Stage1Button->SetVisibility(ESlateVisibility::Hidden);
	Stage1->SetVisibility(ESlateVisibility::Hidden);
	Stage2Button->SetVisibility(ESlateVisibility::Hidden);
	Stage2->SetVisibility(ESlateVisibility::Hidden);
	Stage3Button->SetVisibility(ESlateVisibility::Hidden);
	Stage3->SetVisibility(ESlateVisibility::Hidden);
	BackButton->SetVisibility(ESlateVisibility::Hidden);
	Back->SetVisibility(ESlateVisibility::Hidden);
	MissionText->SetVisibility(ESlateVisibility::Hidden);
}

void UMainStartUI::Stage1ButtonHovered()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("OnHovered 1"));
	//StageImage->SetBrushFromTexture(MapText1, false);
	StageImage->SetBrushFromMaterial(MapMat1);
	FName Text;
	FName Mission;
	Mission = FName(TEXT("잠입"));
	Text = FName(TEXT("깊은 산골에 있는 데저트 울프의 본기지.\n심상치 않은 모의를 하고 있는 것 같다.\n은밀하게 침투하여 조사하라."));
	MissionText->SetText(FText(FText::FromName(Mission)));
	MissionText->SetVisibility(ESlateVisibility::Visible);
	StageText->SetText(FText(FText::FromName(Text)));
	StageText->SetVisibility(ESlateVisibility::Visible);
	StageImage->SetVisibility(ESlateVisibility::Visible);
	
	// StageImage->SetBrushFromMaterial(MapMat2);
	// FName Text;
	// FName Mission;
	// Mission = FName(TEXT("해킹"));
	// Text = FName(TEXT("데저트 울프의 본기지에 무사히 잠입했고,\n그 곳에는 거대한 미사일이 배치되어 있다.\n해킹을 통해 음모를 파악하라."));
	// MissionText->SetText(FText(FText::FromName(Mission)));
	// MissionText->SetVisibility(ESlateVisibility::Visible);
	// StageText->SetText(FText(FText::FromName(Text)));
	// StageText->SetVisibility(ESlateVisibility::Visible);
	// StageImage->SetVisibility(ESlateVisibility::Visible);
}

void UMainStartUI::Stage2ButtonHovered()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("OnHovered 2"));
	//StageImage->SetBrushFromTexture(MapText2, false);
	StageImage->SetBrushFromMaterial(MapMat2);
	FName Text;
	FName Mission;
	Mission = FName(TEXT("해킹"));
	Text = FName(TEXT("데저트 울프의 본기지에 무사히 잠입했고,\n그 곳에는 거대한 미사일이 배치되어 있다.\n해킹을 통해 음모를 파악하라."));
	MissionText->SetText(FText(FText::FromName(Mission)));
	MissionText->SetVisibility(ESlateVisibility::Visible);
	StageText->SetText(FText(FText::FromName(Text)));
	StageText->SetVisibility(ESlateVisibility::Visible);
	StageImage->SetVisibility(ESlateVisibility::Visible);
	
	// StageImage->SetBrushFromMaterial(MapMat3);
	// FName Text;
	// FName Mission;
	// Mission = FName(TEXT("저지"));
	// Text = FName(TEXT("미사일을 테러 조직에 판매하려는 음모는\n파악했지만, 지원을 요청할 시간은 없다.\n기차가 거래 장소에 도착하기 전에,\n카드키를 탈취하여 음모를 저지하라."));
	// MissionText->SetText(FText(FText::FromName(Mission)));
	// MissionText->SetVisibility(ESlateVisibility::Visible);
	// StageText->SetText(FText(FText::FromName(Text)));
	// StageText->SetVisibility(ESlateVisibility::Visible);
	// StageImage->SetVisibility(ESlateVisibility::Visible);
}

void UMainStartUI::Stage3ButtonHovered()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("OnHovered 3"));
	
	StageImage->SetBrushFromMaterial(MapMat3);
	FName Text;
	FName Mission;
	Mission = FName(TEXT("저지"));
	Text = FName(TEXT("미사일을 테러 조직에 판매하려는 음모는\n파악했지만, 지원을 요청할 시간은 없다.\n기차가 거래 장소에 도착하기 전에,\n카드키를 탈취하여 음모를 저지하라."));
	MissionText->SetText(FText(FText::FromName(Mission)));
	MissionText->SetVisibility(ESlateVisibility::Visible);
	StageText->SetText(FText(FText::FromName(Text)));
	StageText->SetVisibility(ESlateVisibility::Visible);
	StageImage->SetVisibility(ESlateVisibility::Visible);
}

void UMainStartUI::Stage1ButtonOffHovered()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("OffHovered 1"));
	//StageImage->SetVisibility(ESlateVisibility::Hidden);
	StageImage->SetBrushFromMaterial(MapMat4);
	StageText->SetVisibility(ESlateVisibility::Hidden);
	MissionText->SetVisibility(ESlateVisibility::Hidden);
}

void UMainStartUI::Stage2ButtonOffHovered()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("OffHovered 2"));
	//StageImage->SetVisibility(ESlateVisibility::Hidden);
	StageImage->SetBrushFromMaterial(MapMat4);
	StageText->SetVisibility(ESlateVisibility::Hidden);
	MissionText->SetVisibility(ESlateVisibility::Hidden);
}

void UMainStartUI::Stage3ButtonOffHovered()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("OffHovered 3"));
	//StageImage->SetVisibility(ESlateVisibility::Hidden);
	StageImage->SetBrushFromMaterial(MapMat4);
	StageText->SetVisibility(ESlateVisibility::Hidden);
	MissionText->SetVisibility(ESlateVisibility::Hidden);
}

void UMainStartUI::DeleteSave()
{
	UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	inst->DeleteSaveData();
}

void UMainStartUI::BackMusicOn()
{
	if(IsValid(AudioControll))
		AudioControll->Play();
}

void UMainStartUI::BackMusicOff()
{
	if(IsValid(AudioControll))
		AudioControll->Stop();
}