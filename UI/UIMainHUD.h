// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "Styling/SlateBrush.h"
//#include "WeaponComponent.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "Components/BackgroundBlur.h"
#include "Blueprint/UserWidget.h"
#include "UIMainHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPECTER_API UUIMainHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UProgressBar* HPBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UProgressBar* GrenadeCoolTimeBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UBorder* WeaponInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UBackgroundBlur* BackGroundBlur;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* WeaponSwapBackGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* BulletCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* Slash;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* BulletAllCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Weapon1BackGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Weapon2BackGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Weapon3BackGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* WeaponName1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* WeaponName2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* WeaponName3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Weapon1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Selector1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Selector2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Selector3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Selector4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Selector5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Weapon11;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Weapon22;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Weapon33;

	UPROPERTY()
		float blur;

	UPROPERTY()
		bool CheckWeapon1 = true;
	UPROPERTY()
		bool CheckWeapon2 = false;
	UPROPERTY()
		bool CheckWeapon3 = false;
	UPROPERTY()
		bool RifleCheck = false;


	UPROPERTY()
		FTimerHandle timer;
	void SetHP(float fPercent);
	void SetGrenadeCoolTime(float fPercent);
	void SetBullet(class FText bullet);
	void SetBulletAll(class FText allbullet);

	void SetWeapon(class UTexture2D* _Gun, FName _Name);
	void SetWeapon2(class UTexture2D* _Gun, FName _Name);
	void SetWeapon3(class UTexture2D* _Gun, FName _Name);
	void ExistingWeaponOn();
	void ExistingWeaponOff();

	void Weapon1PickVisi();
	void Weapon2PickVisi();
	void Weapon3PickVisi();

	void RifleSingle();
	void RifleBurst();
	void RifleFullAuto();
	void RifleOff();


	void BackGroundVisi(class UTexture2D* _GunText1, class UTexture2D* _GunText2, class UTexture2D* _GunText3, FName _Name1, FName _Name2, FName _Name3);
	void BackGroundOffVisi();
	void SetBlur(float InStrength);

protected:
	virtual void NativeConstruct() override;
};
