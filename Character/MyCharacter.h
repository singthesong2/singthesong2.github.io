// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define Max_Walk 300

#include "CoreMinimal.h"
#include "MyCharacterBase.h"
#include "MyCharacterAI_Controller.h"
#include "InputActionValue.h"
#include "Components/TimeLineComponent.h"
#include "../Event/SaveLoad/SavableInterface.h"
#include "../WeaponComp/WeaponToOwner.h"
#include "MyCharacter.generated.h"

UCLASS()
class SPECTER_API AMyCharacter : public AMyCharacterBase, public IWeaponToOwner, public ISavableInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleCollider;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* LeftCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Colision, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ourParitcleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWeaponComponent> Weapon_Component;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WD, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> AmsalClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> TrainCapsule;
	
public:
	// Sets default values for this character's properties
	AMyCharacter();

	// 우원씨가 추가한거
	void StartShoot();
	void StopShoot();
	void StopDamage();

	void Weapon1SwapSwitch();
	void Weapon2SwapSwitch();
	void Weapon3SwapSwitch();

	void BackGroundOffSwitch();
	void BackGround();

	float targetArmLength = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WD, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* AmsalWD;

	UPROPERTY(EditAnywhere)
	UCurveFloat* TimeLineCurve;

	UPROPERTY(EditAnywhere)
	UCurveFloat* TimeLineAimingCurve;

	UPROPERTY(EditAnywhere)
	UCurveFloat* TimeCrouchingCapsuleCurve;

	UPROPERTY(EditAnywhere)
	UCurveFloat* TimeCrouchingCameraCurve;

	UPROPERTY(EditAnywhere)
	UCurveFloat* TimeLineCrouchingArmCurve;

	UPROPERTY(EditAnywhere)
	UCurveFloat* TimeLineSprintArmCurve;

	UPROPERTY(EditAnywhere)
	UAnimMontage* GrenadeMontage;

private:
	FTimeline timeLine;
	FOnTimelineFloat TimeLineUpdatedelegate;

	FTimeline timeLineAiming;
	FOnTimelineFloat TimeLineAimingUpdatedelegate;

	FTimeline SmoothCrouchingCurveTimeLine;
	FTimeline SmoothCrouchingTargetArm;

	FOnTimelineFloat TimeLineCrouchCapsuleUpdatedelegate;
	FOnTimelineFloat TimeLineCrouchCameraUpdatedelegate;
	FOnTimelineFloat TimeLineCrouchArmUpdatedelegate;

	FTimeline SmoothSprintCurveTimeLine;
	FOnTimelineFloat TimeLineSprintArmUpdatedelegate;

	UPROPERTY()
	class AMyCharacterAI* AiPawn;
	UPROPERTY()
	class AMyCharacterAI_Controller* OtherAI;

	UPROPERTY(VisibleAnywhere, Category = Flag)
	AActor* Flag = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Flag)
	FVector PlayerLoc = FVector::ZeroVector;

	UFUNCTION()
	void TimeLineUpdateFunc(float Output);

	UFUNCTION()
	void TimeLineSprintUpdateFunc(float Output);

	UFUNCTION()
	void TimeLineCrouchArmUpdateFunc(float Output);

	UFUNCTION()
	void TimeLineAimingUpdateFunc(float Output);

	UFUNCTION()
	void TimeLineCrouchUpdateFunc(float Capsule);

	UFUNCTION()
	void TimeLineCrouchCameraUpdateFunc(float Camera);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	bool Is_Spirnt = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	bool Is_crouch = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	bool Is_Zoom = false;

	UPROPERTY(VisibleAnywhere, Category = PawnState)
	float Forward;

	UPROPERTY(VisibleAnywhere, Category = PawnState)
	bool ChangeCameraPos;

	UPROPERTY(VisibleAnywhere, Category = PawnState)
	bool MoveDirection;

	UPROPERTY(VisibleAnywhere, Category = PawnState)
	float Left;

	float LeftSpring = 90;
	float CrouchSpring = 80;

	UPROPERTY()
	class UCharacte_Ani* Anim;

	UFUNCTION()
	void OffSet_AIm();

	UFUNCTION()
	void CameraWibbingFun();

	float Max_Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnRoll", meta = (AllowPrivateAccess = "true"))
	bool Is_Roll;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnValut", meta = (AllowPrivateAccess = "true"))
	bool Is_Valut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnValut", meta = (AllowPrivateAccess = "true"))
	bool Is_CharacterCanFire = true;

	UPROPERTY(VisibleAnywhere)
	float opval = 0.0f;

	FTimerHandle timer;
	UPROPERTY(VisibleAnywhere)
	bool isFiring;

	UPROPERTY(VisibleAnywhere)
	bool isDamage;

	UPROPERTY()
	float Time = 0.0f;

	UPROPERTY()
	float KeyTime = 0.0f;

	float t;

	UPROPERTY(VisibleAnywhere)
	bool ChooseWeapon1 = true;
	UPROPERTY(VisibleAnywhere)
	bool ChooseWeapon2 = false;
	UPROPERTY(VisibleAnywhere)
	bool ChooseWeapon3 = false;

	UPROPERTY()
	bool isSwap;

	UPROPERTY()
	int32 RandomNum;

	UFUNCTION()
	void Aiming();

	UFUNCTION()
	void AimingEnd();
	
public:
	UPROPERTY()
	TObjectPtr<class ATrain_Between> Actor_Train;
	UPROPERTY()
	TObjectPtr<AActor> PreActor;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = State)
	bool Dead = false;

	void OnInteraction();

	void Fire();

	void FireRelease();

	void ChangeSelector();

	void ChangeSlot1();

	void ChangeSlot2();

	void ChangeSlot3();

	void ReloadStart();

	void DecisionOffTime();

	bool ButtonOnOff;

	void AddAmmo(int _slot, int _Amount);

	void ThrowMode();

	void ReadytoThrow();

	void Throw();

	UFUNCTION(BlueprintCallable)
	void ThrowObject_Notify();

	UFUNCTION(BlueprintCallable)
	void StopThrow();

	void ShowProjectilePath();

	void ClearSplineMesh();

	void StopPauseOn();
	bool StopPauseSwitch = false;

	UFUNCTION(BlueprintCallable)
	void ClearGrenadeCoolTime();

	FVector CalculateThrowVelocity();

	TObjectPtr<class UWeaponComponent> GetWeaponComponent() { return Weapon_Component; }

	UFUNCTION(BlueprintCallable)
	void ThrowObjectSpawn_Notify();

	UFUNCTION(BlueprintCallable)
	void ThrowReady_Notify();

	/*virtual void PlayReloadAnimation() override;
	virtual void PlayFireAnimation() override;
	virtual void PlaySlotHoldAnimation(EWeaponSlot _Slot) override;
	virtual void PlaySlotReleaseAnimation(EWeaponSlot _Slot) override;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TArray<TSubclassOf<class AGunBase>> GunSlot;

	bool throwObjectReady;

	bool Is_Throwing;

	bool IsGetThrowObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	bool IsGrenadeCoolTimeReady = true;

	float CurGrenadeCoolTime;

	float MaxGrenadeCoolTime = 5.f;

	float ThrowPower;

	class USplineComponent* ProjectilePath;

	TArray<class USplineMeshComponent*> Spline_Meshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	UMaterial* ProjectilePathMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	UStaticMesh* ProjectileDefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	UStaticMeshComponent* ProjectileEndSphere;

	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileStart;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AThrowableObject> ThrowObject;

	AThrowableObject* SpawnThrowObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = PawnState)
	int AmsalNum = 0;

	void SetActorFlag(AActor* Loc1);

	// 우원씨가 추가한거
	// 내가 추가한거
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HeadDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BodyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ArmLegDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ResetHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Shoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Reload;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UUIMainHUD* MainHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UDamageUI* DamageHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UStopPauseUI* StopPauseHUDWidget;

	

	class UCrossHairUI* CrossHairWidget;

	class UUserWidget* AmsalWidget;

	class UUserWidget* SniperWidget;

	UPROPERTY()
	float ChHPopacity;

	bool CheckAmsalResult;

	bool flag = false;
	bool Is_LeftButtonOn = false;

	FVector Direction2;

	FVector Actor_Loc;
	FRotator Player_Way;

	FVector Camera_Loc;
	FVector CameraFor_Loc;
	
	int32 DeathTime = 0;
	bool Play_Amsal = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = "true"))
	bool PlayerDeath = false;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class UWeaponComponent* WeaponSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GunValue", Meta = (AllowPrivateAccess = true))
	EWeaponSlot TempEWeapon;
	virtual void PlayReloadAnimation() override;	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "PlayReloadAnimation", ScriptName = "PlayReloadAnimation"))
	void K2_PlayReloadAnimation();
	virtual void K2_PlayReloadAnimation_Implementation() {};

	virtual void PlayFireAnimation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "PlayFireAnimation", ScriptName = "PlayFireAnimation"))
	void K2_PlayFireAnimation();
	virtual void K2_PlayFireAnimation_Implementation() {};

	virtual void PlaySlotHoldAnimation(EWeaponSlot _Slot) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "PlaySlotHoldAnimation", ScriptName = "PlaySlotHoldAnimation"))
	void K2_PlaySlotHoldAnimation(EWeaponSlot _Slot);
	virtual void K2_PlaySlotHoldAnimation_Implementation(EWeaponSlot _Slot) {};

	virtual void PlaySlotReleaseAnimation(EWeaponSlot _Slot) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "PlaySlotReleaseAnimation", ScriptName = "PlaySlotReleaseAnimation"))
	void K2_PlaySlotReleaseAnimation(EWeaponSlot _Slot);
	virtual void K2_PlaySlotReleaseAnimation_Implementation(EWeaponSlot _Slot) {};

	virtual void PlayGrenadeThrowAnimation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "PlayGrenadeThrowAnimation", ScriptName = "PlayGrenadeThrowAnimation"))
	void K2_PlayGrenadeThrowAnimation();
	virtual void K2_PlayGrenadeThrowAnimation_Implementation() {};

	virtual void PlayGrenadeThrowEndAnimation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "PlayGrenadeThrowEndAnimation", ScriptName = "PlayGrenadeThrowEndAnimation"))
	void K2_PlayGrenadeThrowEndAnimation();
	virtual void K2_PlayGrenadeThrowEndAnimation_Implementation() {};

	virtual void StopGrenadeThrowAnimation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "StopGrenadeThrowAnimation", ScriptName = "StopGrenadeThrowAnimation"))
	void K2_StopGrenadeThrowAnimation();
	virtual void K2_StopGrenadeThrowAnimation_Implementation() {};

	virtual void PlayOwnerFireAnimation(EWeaponType _Type) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "PlayOwnerFireAnimation", ScriptName = "PlayOwnerFireAnimation"))
	void K2_PlayOwnerFireAnimation(EWeaponType _Type);
	virtual void K2_PlayOwnerFireAnimation_Implementation(EWeaponType _Type) {};

	UFUNCTION()
	void TriggerEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TriggerExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void FPP_Mode();

	void WheelEvent(float Value);

	void LButtonDown();

	void LButtonRelease();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void TurnLook(float VALUE);
	void LookUp(float VALUE);

	void MoveForward(float VALUE);
	void LeftRight(float VALUE);

	UFUNCTION()
	void EndMontaged(UAnimMontage* Montage, bool bInterrupted);

	//앉기 기능구현
	void StartCrouch();

	//구르기 기능 구현
	void Start_Roll();

	//Sprint
	void Do_Sprint();

	void End_Sprint();

	void CheckAmsal();

	void StartAmsal();

	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	class UDataTable* AniTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UPauseUI* PauseHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	FRotator Camera_Way;

	virtual void SetActorToSavableList_Implementation() override;
	virtual FString JsonSerialize_Implementation() override;
	virtual bool JsonDeserialize_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
	float Crouch_Walkspeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
	float Sprint_Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PawnState)
	float Aim_Yaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PawnState)
	float Aim_Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PawnState)
	bool EventOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PawnState)
	float Search_Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PawnState)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerState)
	bool MoveCharacter = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PawnState", meta = (AllowPrivateAccess = "true"))
	int Get_Weapon = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAmsal;

	UFUNCTION()
	void Player_FlagMontage();

	UFUNCTION()
	void Noti_FlagMontage();

	UFUNCTION(BlueprintImplementableEvent)
	void Amsal_BP();

	void DeathNotify();

	FORCEINLINE TObjectPtr<class USpringArmComponent> GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE TObjectPtr<class UCameraComponent> GetLeftCamera() const { return LeftCamera; }
	inline bool Get_crocuhState() { return Is_crouch; }
	inline bool Get_SprintState() { return Is_Spirnt; }

	inline float Get_Forward() { return Forward; }
	inline float Get_Left() { return Left; }
	inline float Get_Yaw() { return Aim_Yaw; }
	inline float Get_Pitch() { return Aim_Pitch; }
	inline bool Get_Direction() { return MoveDirection; }
	inline UDataTable* Get_Table() { return AniTable; }

	UFUNCTION(BlueprintImplementableEvent)
	void Vision(bool VisionSet);
	void UpdateVisionState(const bool& NewState);
	bool IsVisionStateModified(const bool& NewState) const;
	bool GetNightVisionState();
protected:
	UPROPERTY(BlueprintReadWrite)
	bool NightVisionState;
	void LoadData();
	FVector LastSavedLocation;
public:
	void SetSavedLocation(const FVector& SaveLocation);
	FVector GetSavedLocation();
};