// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "UIGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Event/SaveLoad/PlayerSaveData.h"
#include "MyCharacterAI_Controller.h"
#include "MyCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Characte_Ani.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "WeaponComponent.h"
#include "ADS_Camera.h"
#include "Floor.h"
#include "MyCharacterAI.h"
#include "UIMainHUD.h"
#include "CrossHairUI.h"
#include "ThrowableObject.h"
#include "Throwable_Rock.h"
#include "DamageUI.h"
#include "OpticsBase.h"
#include "Train_Between.h"
#include "UI/StopPauseUI.h"
#include "UI/PauseUI.h"
#include "Train_Rail.h"
#include "../Event/DialogueInstance.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
	RootComponent = GetCapsuleComponent();

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	GetCharacterMovement()->MaxFlySpeed = 150.f;
	GetCharacterMovement()->BrakingDecelerationFlying = 500.f;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = targetArmLength; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	LeftCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("LeftCamera"));
	LeftCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	LeftCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("None"));

	Sprint_Speed = 600.f;
	Search_Distance = 30.f;

	DeathTime = 0;
	ThrowPower = 700.f;

	Weapon_Component = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon_Component"));
	AmsalWD = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmsalWD"));
	
	ConstructorHelpers::FClassFinder<UUserWidget>WD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WeaponSystem/WIdget/WD_Amsal.WD_Amsal_C'"));
	if (WD.Succeeded())
	{
		AmsalWD->SetWidgetSpace(EWidgetSpace::Screen);
		AmsalWD->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AmsalWD->SetWidgetClass(WD.Class);
		AmsalWD->SetDrawAtDesiredSize(true);
		AmsalWD->SetupAttachment(RootComponent);
		AmsalWD->InitWidget();
	}

	ProjectileStart = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile_Start"));
	ProjectileStart->SetupAttachment(RootComponent);
	//ProjectileStart->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("hand_l"));

	ProjectilePath = CreateDefaultSubobject<USplineComponent>(TEXT("ProjectilePath"));

	ProjectileEndSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileEnd"));
	ProjectileEndSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileEndSphere->SetVisibility(false);

	
	TrainCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeshCapsule"));
	TrainCapsule->InitCapsuleSize(34.f, 88.0f);
	TrainCapsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	TrainCapsule->CanCharacterStepUpOn = ECB_No;
	TrainCapsule->SetShouldUpdatePhysicsVolume(true);
	TrainCapsule->SetCanEverAffectNavigation(false);
	TrainCapsule->bDynamicObstacle = true;
	TrainCapsule->SetupAttachment(GetCapsuleComponent());

	AmsalWD->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ProjectileStart->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ProjectilePath->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ProjectileEndSphere->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	CameraBoom->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	AmsalWD->SetupAttachment(TrainCapsule);
	ProjectileStart->SetupAttachment(TrainCapsule);
	ProjectilePath->SetupAttachment(TrainCapsule);
	ProjectileEndSphere->SetupAttachment(TrainCapsule);
	CameraBoom->SetupAttachment(TrainCapsule);
	GetMesh()->SetupAttachment(TrainCapsule);
	NightVisionState = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(GetActorLocation() + FVector(1, 1, 0));
	LoadData();
	Dead = false;
	SetActorToSavableList_Implementation();
	FVector CompLocation = GetActorLocation();
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, CompLocation, CompLocation - FVector(0, 0, 1000), ECollisionChannel::ECC_GameTraceChannel2, CollisionParams)) {
		//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("%s"), *Hit.GetActor()->GetName()));
		if (Hit.GetActor()->IsA(ATrain_Between::StaticClass()) == true) {
			if (Actor_Train != Hit.GetActor()) {
				Actor_Train = Cast<ATrain_Between>(Hit.GetActor());
				TrainMap = true;
			}
		}
		else if (Hit.GetActor()->IsA(ATrain_Rail::StaticClass()) == true) {
			TObjectPtr<ATrain_Rail> TR = Cast<ATrain_Rail>(Hit.GetActor());
			if (Actor_Train != TR->Train_Between) {
				FRotator RotTrain = TR->GetActorRotation() + FRotator(0, 90, 0);
				RotTrain *= -1;
				FVector Reposition = TR->Train_Between->GetActorLocation() - RotTrain.RotateVector(TR->GetActorLocation() - TrainCapsule->GetComponentLocation());
				SetActorLocation(Reposition, false, nullptr, ETeleportType::None);
				Actor_Train = TR->Train_Between;
				//DrawDebugCircle(GetWorld(), Reposition, 1000, 20, FColor::Green, false, 100, 0, 10);

				FRotator tVecRot = Actor_Train->Rail_Train->GetActorRotation() + FRotator(0, 90, 0);
				FVector tVec = Actor_Train->Rail_Train->GetActorLocation() - tVecRot.RotateVector(Actor_Train->GetActorLocation() - GetActorLocation());
				FRotator tRot;
				tRot.Roll = GetActorRotation().Roll;
				tRot.Pitch = GetActorRotation().Pitch;
				tRot.Yaw = Actor_Train->Rail_Train->GetActorRotation().Yaw + GetActorRotation().Yaw + 90;
				TrainCapsule->SetWorldLocation(tVec, false, nullptr, ETeleportType::TeleportPhysics);
				TrainCapsule->SetWorldRotation(tRot, false, nullptr, ETeleportType::TeleportPhysics);
				TrainMap = true;
			}
		}
	}
	//}
	//DrawDebugLine(GetWorld(), CompLocation, Hit.bBlockingHit == true ? Hit.Location : CompLocation - FVector(0, 0, 160), Hit.bBlockingHit == true ? FColor::Green : FColor::Red, false, DeltaTime * 2, 0, 10);

	if (IsValid(PreActor) == false) {
		if (IsValid(Actor_Train) == true) {
			TObjectPtr<ATrain_Rail> PreTick = Actor_Train->Rail_Train;
			while (true) {
				if (IsValid(PreTick) == true) {
					if (IsValid(PreTick->Back_Train) == true) {
						PreTick = PreTick->Back_Train;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			PreActor = PreTick;
			AddTickPrerequisiteActor(PreActor);
		}
	}



	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	AmsalWD->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(false);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Speed = 300.f;

	TimeLineUpdatedelegate.BindUFunction(this, FName("TimeLineUpdateFunc"));
	timeLine.AddInterpFloat(TimeLineCurve, TimeLineUpdatedelegate);
	timeLine.SetTimelineLength(2.0f);

	TimeLineAimingUpdatedelegate.BindUFunction(this, FName("TimeLineAimingUpdateFunc"));
	timeLineAiming.AddInterpFloat(TimeLineAimingCurve, TimeLineAimingUpdatedelegate);
	timeLineAiming.SetTimelineLength(1.0f);

	TimeLineSprintArmUpdatedelegate.BindUFunction(this, FName("TimeLineSprintUpdateFunc"));
	SmoothSprintCurveTimeLine.AddInterpFloat(TimeLineSprintArmCurve, TimeLineSprintArmUpdatedelegate);
	SmoothSprintCurveTimeLine.SetTimelineLength(0.3f);

	TimeLineCrouchCapsuleUpdatedelegate.BindUFunction(this, FName("TimeLineCrouchUpdateFunc"));
	TimeLineCrouchCameraUpdatedelegate.BindUFunction(this, FName("TimeLineCrouchCameraUpdateFunc"));
	TimeLineCrouchArmUpdatedelegate.BindUFunction(this, FName("TimeLineCrouchArmUpdateFunc"));

	SmoothCrouchingCurveTimeLine.AddInterpFloat(TimeCrouchingCapsuleCurve, TimeLineCrouchCapsuleUpdatedelegate);
	SmoothCrouchingCurveTimeLine.AddInterpFloat(TimeCrouchingCameraCurve, TimeLineCrouchCameraUpdatedelegate);
	SmoothCrouchingTargetArm.AddInterpFloat(TimeLineCrouchingArmCurve, TimeLineCrouchArmUpdatedelegate);

	SmoothCrouchingTargetArm.SetTimelineLength(0.3f);

	// 내가 추가한거
	// 현재 월드의 게임 모드를 받아온다
	AUIGameMode* GameMode = Cast<AUIGameMode>(GetWorld()->GetAuthGameMode());
	// 게임 모드에서 위젯을 받아온다
	MainHUDWidget = GameMode->GetMainHUDWidget();
	DamageHUDWidget = GameMode->GetDamageHUDWidget();
	StopPauseHUDWidget = GameMode->GetStopPauseHUDWidget();
	CrossHairWidget = GameMode->GetCrossHairWidget();
	SniperWidget = GameMode->GetSniperWidget();
	AmsalWidget = GameMode->GetAmsalWidgetclass();
	PauseHUDWidget = GameMode->GetPauseHUDWidget();

	CrossHairWidget->SetVisibility(ESlateVisibility::Visible);
	CrossHairWidget->SetCrossHairVisible(false);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::TriggerEnter);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::TriggerExit);

	Weapon_Component->InitalizeWeaponComponent(FollowCamera, GunSlot[0], GunSlot[1], GunSlot[2], true);
	Weapon_Component->LaserSightOn = false;

	if (GunSlot[0])
	{
		PlaySlotReleaseAnimation(EWeaponSlot::WeaponSlot1);
		Get_Weapon = 1;
	}
	if (true == NightVisionState)
	{
		Vision(NightVisionState);
	}
	//ChangeSlot1();
	//DecisionOffTime();
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Anim = Cast<UCharacte_Ani>(GetMesh()->GetAnimInstance());

	if (Anim != nullptr)
	{
		Anim->OnMontageEnded.AddDynamic(this, &AMyCharacter::EndMontaged);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CompLocation = GetActorLocation();
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

    if (!Play_Amsal)
	{
			CheckAmsal();
	}

	if (HP <= 0 && !Dead)
	{
		Anim->PlayMontage(FName("Death"));
		Dead = true;
	}

	if (IsValid(Actor_Train) == false)
	{
		FVector Start = TrainCapsule->GetComponentLocation() + GetActorUpVector() * CrouchSpring;
		//if(!Is_crouch)	Start = GetActorLocation() + GetActorUpVector() * 80.f;
		FVector End = FollowCamera->GetRightVector() * LeftSpring;
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, Start + End, ECC_Camera, Params);
		FVector CamVec = Start + (HitResult.bBlockingHit ?
			End * (HitResult.Time - 0.1) : End * 0.9);
		CameraBoom->SetWorldLocation(CamVec);
		//DrawDebugLine(GetWorld(), GetActorLocation() + Start, GetActorLocation() + End, FColor::Red, false, DeltaTime * 2, 0, 5);
	}
	
	
	if (IsValid(Actor_Train) == true) {
		if (IsValid(Actor_Train->Rail_Train) == true) {
			FRotator tVecRot = Actor_Train->Rail_Train->GetActorRotation() + FRotator(0, 90, 0);
			FVector tVec = Actor_Train->Rail_Train->GetActorLocation() - tVecRot.RotateVector(Actor_Train->GetActorLocation() - GetActorLocation());
			FRotator tRot;
			tRot.Roll = GetActorRotation().Roll;
			tRot.Pitch = GetActorRotation().Pitch;
			tRot.Yaw = Actor_Train->Rail_Train->GetActorRotation().Yaw + GetActorRotation().Yaw + 90;
			TrainCapsule->SetWorldLocation(tVec, false, nullptr, ETeleportType::TeleportPhysics);
			TrainCapsule->SetWorldRotation(tRot, false, nullptr, ETeleportType::TeleportPhysics);

			CompLocation = TrainCapsule->GetComponentLocation();
			GetController()->SetControlRotation(GetControlRotation() + (Actor_Train->Rail_Train->Rotator_Cur - Actor_Train->Rail_Train->Rotator_Prv));
		}
	}
	else {
		TrainCapsule->SetWorldLocation(GetActorLocation());
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(GetCharacterMovement()->MovementMode)));
	//if (GetCharacterMovement()->MovementMode == MOVE_Walking) {
		if (GetWorld()->LineTraceSingleByChannel(Hit, CompLocation, CompLocation - FVector(0, 0, 1000), ECollisionChannel::ECC_GameTraceChannel2, CollisionParams)) {
			//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("%s"), *Hit.GetActor()->GetName()));
			if (Hit.GetActor()->IsA(ATrain_Between::StaticClass()) == true) {
				if (Actor_Train != Hit.GetActor()) {
					Actor_Train = Cast<ATrain_Between>(Hit.GetActor());
				}
			}
			else if (Hit.GetActor()->IsA(ATrain_Rail::StaticClass()) == true) {
				TObjectPtr<ATrain_Rail> TR = Cast<ATrain_Rail>(Hit.GetActor());
				if (Actor_Train != TR->Train_Between) {
					FRotator RotTrain = TR->GetActorRotation() + FRotator(0, 90, 0);
					RotTrain *= -1;
					FVector Reposition = TR->Train_Between->GetActorLocation() - RotTrain.RotateVector(TR->GetActorLocation() - TrainCapsule->GetComponentLocation());
					SetActorLocation(Reposition, false, nullptr, ETeleportType::None);
					Actor_Train = TR->Train_Between;
					//DrawDebugCircle(GetWorld(), Reposition, 1000, 20, FColor::Green, false, 100, 0, 10);

					FRotator tVecRot = Actor_Train->Rail_Train->GetActorRotation() + FRotator(0, 90, 0);
					FVector tVec = Actor_Train->Rail_Train->GetActorLocation() - tVecRot.RotateVector(Actor_Train->GetActorLocation() - GetActorLocation());
					FRotator tRot;
					tRot.Roll = GetActorRotation().Roll;
					tRot.Pitch = GetActorRotation().Pitch;
					tRot.Yaw = Actor_Train->Rail_Train->GetActorRotation().Yaw + GetActorRotation().Yaw + 90;
					TrainCapsule->SetWorldLocation(tVec, false, nullptr, ETeleportType::TeleportPhysics);
					TrainCapsule->SetWorldRotation(tRot, false, nullptr, ETeleportType::TeleportPhysics);
				}
			}
		}
	//}
	//DrawDebugLine(GetWorld(), CompLocation, Hit.bBlockingHit == true ? Hit.Location : CompLocation - FVector(0, 0, 160), Hit.bBlockingHit == true ? FColor::Green : FColor::Red, false, DeltaTime * 2, 0, 10);

	if (IsValid(PreActor) == false) {
		if (IsValid(Actor_Train) == true) {
			TObjectPtr<ATrain_Rail> PreTick = Actor_Train->Rail_Train;
			while(true) {
				if (IsValid(PreTick) == true) {
					if (IsValid(PreTick->Back_Train) == true) {
						PreTick = PreTick->Back_Train;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			PreActor = PreTick;
			AddTickPrerequisiteActor(PreActor);
		}
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0));


	
	timeLine.TickTimeline(DeltaTime);
	timeLineAiming.TickTimeline(DeltaTime);
	SmoothCrouchingCurveTimeLine.TickTimeline(DeltaTime);
	SmoothCrouchingTargetArm.TickTimeline(DeltaTime);
	SmoothSprintCurveTimeLine.TickTimeline(DeltaTime);

	Camera_Way = FollowCamera->GetComponentRotation();

	if (Play_Amsal)
		GetCharacterMovement()->DisableMovement();

	//Time += DeltaTime;
	if (ButtonOnOff)
	{
		KeyTime += DeltaTime;

		if (KeyTime > 0.5f)
			BackGround();
	}

	if (!ButtonOnOff)
	{
		if (KeyTime > 0.5f)
			BackGroundOffSwitch();
		KeyTime = 0.0f;
	}

	if (IsValid(MainHUDWidget))
	{
		if (Weapon_Component->IsDamage == false)
		{
			Time += DeltaTime;
			ChHPopacity = HP / HP_Max;

			if (Time >= 5.0f)
			{
				if (HP <= HP_Max)
				{
					HP += 1.0f;
					MainHUDWidget->SetHP(HP / HP_Max);
					//DamageHUDWidget->SetOp(HP / HP_Max);
					DamageHUDWidget->SetOp(1.0f - ChHPopacity);
				}
			}
		}
		else if (Weapon_Component->IsDamage == true)
			Time = 0.0f;
	}

	//if(HP==0.0f)
	//	PauseHUDWidget->DeadPauseOn();

	if (IsGrenadeCoolTimeReady == false)
	{
		if (CurGrenadeCoolTime >= MaxGrenadeCoolTime)
		{
			IsGrenadeCoolTimeReady = true;
		}
		else
		{
			CurGrenadeCoolTime += DeltaTime;
		}

		MainHUDWidget->SetGrenadeCoolTime(CurGrenadeCoolTime / MaxGrenadeCoolTime);
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyCharacter::LeftRight);

	InputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);
	InputComponent->BindAxis("LookTurn", this, &AMyCharacter::TurnLook);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &AMyCharacter::StartCrouch);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::Do_Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::End_Sprint);

	InputComponent->BindAction("CameraWibbing",IE_Pressed, this, &AMyCharacter::CameraWibbingFun);
	InputComponent->BindAction("Amsal", IE_Pressed, this, &AMyCharacter::StartAmsal);

	//준혁씨꺼
	InputComponent->BindAction("Aiming", IE_Pressed, this, &AMyCharacter::LButtonDown);
	InputComponent->BindAction("Aiming", IE_Released, this, &AMyCharacter::LButtonRelease);

	InputComponent->BindAction("Reload", IE_Pressed, this, &AMyCharacter::ReloadStart);

	InputComponent->BindAction("Switch Selecter", IE_Pressed, this, &AMyCharacter::ChangeSelector);

	InputComponent->BindAction("FPP Mode", IE_Pressed, this, &AMyCharacter::FPP_Mode);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::Fire);
	InputComponent->BindAction("Fire", IE_Released, this, &AMyCharacter::FireRelease);

	//InputComponent->BindAction("Grenade", IE_Pressed, this, &AMyCharacter::ThrowMode);

	//InputComponent->BindAction("Switch Selecter", IE_Pressed, this, &AMyCharacter::ChangeSelector);

	InputComponent->BindAction("Slot1", IE_Pressed, this, &AMyCharacter::ChangeSlot1);
	InputComponent->BindAction("Slot1", IE_Released, this, &AMyCharacter::DecisionOffTime);
	InputComponent->BindAction("Slot2", IE_Pressed, this, &AMyCharacter::ChangeSlot2);
	InputComponent->BindAction("Slot2", IE_Released, this, &AMyCharacter::DecisionOffTime);
	InputComponent->BindAction("Slot3", IE_Pressed, this, &AMyCharacter::ChangeSlot3);
	InputComponent->BindAction("Slot3", IE_Released, this, &AMyCharacter::DecisionOffTime);

	InputComponent->BindAction("StopPause", IE_Pressed, this, &AMyCharacter::StopPauseOn);
	

	InputComponent->BindAxis("WheelMove", this, &AMyCharacter::WheelEvent);

	PlayerInputComponent->BindAction(TEXT("WeaponSwap"), EInputEvent::IE_Pressed, this, &AMyCharacter::BackGround);
	PlayerInputComponent->BindAction(TEXT("WeaponSwap"), EInputEvent::IE_Released, this, &AMyCharacter::BackGroundOffSwitch);
}

void AMyCharacter::SetActorToSavableList_Implementation()
{
	UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	if (nullptr == inst)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to get game instance"));
		return;
	}
	inst->SetActorToList(this);
}

FString AMyCharacter::JsonSerialize_Implementation()
{
	FPlayerSaveData save = FPlayerSaveData(this);
	FString str = save.JsonSerialize();
	//UE_LOG(LogTemp, Log, TEXT("Serialized %s : \n %s"), *GetActorLabel(), *str);
	return str;
}

bool AMyCharacter::JsonDeserialize_Implementation()
{
	UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	TSharedPtr<FJsonObject> json = inst->FindJsonByTag(this->Tags.IsEmpty() ? FName("") : this->Tags[0]);
	if (nullptr == json)
	{
		//UE_LOG(LogTemp, Log, TEXT("Can not find saved data for %s"), *GetName());
		return false;
	}
	//UE_LOG(LogTemp, Log, TEXT("Found saved data for %s"), *GetName());
	FPlayerSaveData data;
	data.SetDeserializedData(json, this);
	return true;
}


void AMyCharacter::MoveForward(float Value)
{
	if (GetCharacterMovement()->IsFalling())
		return;

	if ( Is_Valut || Play_Amsal || MoveCharacter || EventOn || Dead)
		return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);

	Forward = Value;

	AddMovementInput(Direction, Value);
}

void AMyCharacter::LeftRight(float Value)
{
	if (GetCharacterMovement()->IsFalling())
		return;

	if (Play_Amsal ||MoveCharacter || EventOn || Dead)
		return;

	Left = Value;

	if (Value < 0)
		MoveDirection = true;
	else if(Value > 0)
		MoveDirection = false;

	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::LookUp(float Value)
{
	Aim_Pitch = Camera_Way.Pitch;
	AddControllerPitchInput(Value);
}

void AMyCharacter::TurnLook(float Value)
{
	OffSet_AIm();
	AddControllerYawInput(Value);
}

void AMyCharacter::StartCrouch()// 앉기
{
	if ( Play_Amsal || EventOn || Dead)
		return;

	if (!Is_crouch)
	{
		if (Is_Spirnt)
		{
			Is_Spirnt = false;
		}
	
		Is_crouch = true;
		GetCharacterMovement()->MaxWalkSpeed = Crouch_Walkspeed;
		SmoothCrouchingCurveTimeLine.Play();

		if (Is_Zoom)
		{
			timeLineAiming.Play();
		}
		else
			SmoothSprintCurveTimeLine.Play();
			
	}
	else
	{
		Is_crouch = false;
		GetCharacterMovement()->MaxWalkSpeed = Max_Walk;
		SmoothCrouchingCurveTimeLine.Reverse();

		if (Is_Zoom)
		{
			timeLineAiming.Play();
		}
		else
			SmoothSprintCurveTimeLine.Reverse();
	}
}

void AMyCharacter::Aiming()
{
	if (EventOn)
		return;

	if (Is_Spirnt)
	{
		End_Sprint();
	}

	if (Is_Throwing)
	{
		StopThrow();
	}

	if (IsValid(Weapon_Component->CurrentEquipWeapon))
	{
		if (timeLineAiming.IsReversing())
		{
			timeLineAiming.Stop();
		}

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 960.f, 0.f);

		Is_Zoom = true;
		Weapon_Component->IsAiming = true;
		timeLineAiming.Play();
		CrossHairWidget->SetCrossHairVisible(true);
		
	}
}

void AMyCharacter::AimingEnd()
{
	if (Is_Throwing)
		return;

	if (Weapon_Component->ADSMode)
		FPP_Mode();

	Is_Zoom = false;
	Weapon_Component->IsAiming = false;

	FTimerHandle timerHandle;

	CrossHairWidget->SetCrossHairVisible(false);
	FireRelease();

	if (timeLineAiming.IsPlaying())
	{
		timeLineAiming.Stop();
	}

	timeLineAiming.Reverse();
	//SmoothCrouchingCurveTimeLine.Stop();
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f,360.f,0.f);


	//GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
	//	{
	//		if (Is_crouch)
	//		{
	//			SmoothCrouchingCurveTimeLine.Play();
	//		}
	//		else
	//		timeLineAiming.Reverse();
	//	}
	//), 0.1f, false);

}

void AMyCharacter::StartAmsal()// 암살
{
	if (Play_Amsal || EventOn)
		return;
	
	CheckAmsal();

	if (IsAmsal) {
		if (IsValid(AiPawn) == true) {
			if (AiPawn->GetDead() == false) {
				if (Is_Zoom)
					AimingEnd();

				//AmsalWD->SetVisibility(false);
				AmsalWidget->SetVisibility(ESlateVisibility::Hidden);

				Is_crouch = false;
				UnCrouch();

				Play_Amsal = true;

				Amsal_BP();

				TempEWeapon = Weapon_Component->CurrentGunSlot;

				Weapon_Component->CurrentGunSlot = EWeaponSlot::WeaponNone;
				Get_Weapon = 0;
				Weapon_Component->UpdateGunAttachment();

				auto AnimInstance = Cast<UCharacte_Ani>(GetMesh()->GetAnimInstance());
				if (nullptr == AnimInstance)
					return;

				switch (AmsalNum)
				{
				case 1:
					AnimInstance->PlayMontage(FName("Amsal1"));
					break;
				case 2:
					AnimInstance->PlayMontage(FName("Amsal2"));
					break;
				case 3:
					AnimInstance->PlayMontage(FName("Amsal3"));
					break;
				default:
					break;
				}
				
				OtherAI->GetAmsal(AiPawn);
			}
		}
	}
	else
	{
		TArray<AActor*> OverlapedActors;
		GetCapsuleComponent()->GetOverlappingActors(OverlapedActors);

		UE_LOG(LogTemp, Log, TEXT("Input Interaction"));

		Algo::Sort(OverlapedActors, [](AActor* LHS, AActor* RHS) 
			{
				AItemBase* temp = Cast<AItemBase>(LHS);

				if (IsValid(temp))
				{
					return false;
				}

				return true;
			}
		);

		int a = 0;

		for (AActor* Actor : OverlapedActors)
		{
			if (Actor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				IInteractInterface::Execute_OnInteract(Actor, this);
				break;
			}
		}
	}
}

void AMyCharacter::Do_Sprint() //기본Zoom
{
	if (EventOn)
		return;

	if (Is_crouch)
	{
		Is_crouch = false;
		SmoothCrouchingCurveTimeLine.Reverse();
	}
		

	if (Is_Zoom)
	{
		SmoothCrouchingCurveTimeLine.Reverse();
		AimingEnd();
		TempEWeapon = Weapon_Component->CurrentGunSlot;
	}

	Is_Spirnt = true;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;

	SmoothSprintCurveTimeLine.Play();
	GetCharacterMovement()->MaxWalkSpeed = Sprint_Speed;
}

void AMyCharacter::End_Sprint() //기본Zoom
{
	Is_Spirnt = false;
	Speed = Max_Walk;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->bUseControllerDesiredRotation = false;
	if (!Is_crouch && !Is_LeftButtonOn)
	{
		SmoothSprintCurveTimeLine.Reverse();
	}
	GetCharacterMovement()->MaxWalkSpeed = Max_Walk;
}

void AMyCharacter::CameraWibbingFun()
{
	if (!ChangeCameraPos)
	{
		ChangeCameraPos = true;
		timeLine.Play();
	}
	else
	{
		ChangeCameraPos = false;
		timeLine.Reverse();
	}
}

void AMyCharacter::Player_FlagMontage()
{
	auto AnimInstance = Cast<UCharacte_Ani>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance)
		return;

	AnimInstance->PlayMontage(FName("FlagMove"));
}

void AMyCharacter::Noti_FlagMontage()
{
	t = 0;
	PlayerLoc = FVector::ZeroVector;
	MoveCharacter = false;
}

void AMyCharacter::CheckAmsal()
{
	srand(time(NULL));
	FVector Start = GetActorLocation() + GetActorForwardVector() * 50;
	FVector End = Start + GetActorForwardVector() * 15;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	TEnumAsByte<EObjectTypeQuery> WorldPawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(WorldPawn);

	TArray<AActor*> TraceIgnoreActors;
	TraceIgnoreActors.Add(this);

	// 히트 결과 값 받을 변수.
	FHitResult HitResult; // 히트 결과 값 받을 변수.

	CheckAmsalResult = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		Start,
		End,
		ObjectTypes,
		false,
		TraceIgnoreActors, // 무시할 것이 없다고해도 null을 넣을 수 없다.
		EDrawDebugTrace::None,
		HitResult,
		true);

	if (CheckAmsalResult) {
		AiPawn = Cast<AMyCharacterAI>(HitResult.GetActor());

		if (IsValid(AiPawn) == false) {
			IsAmsal = false;
			//AmsalWD->SetVisibility(false);
			AmsalWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			AmsalNum = rand() % 3 + 1;
			OtherAI = Cast<AMyCharacterAI_Controller>(AiPawn->GetController());

			if (AiPawn->GetDead() == false) {
				if (IsValid(OtherAI) == true) {
					if (!OtherAI->Perception_Sight) {
						IsAmsal = true;
						//AmsalWD->SetVisibility(true);
						AmsalWidget->SetVisibility(ESlateVisibility::Visible);
					}
					else {
						IsAmsal = false;
						//AmsalWD->SetVisibility(false);
						AmsalWidget->SetVisibility(ESlateVisibility::Hidden);
					}
				}
			}
		}
	}
	else {
		IsAmsal = false;
		AmsalWidget->SetVisibility(ESlateVisibility::Hidden);

	}
}

void AMyCharacter::OffSet_AIm()
{
	if (GetActorRotation().Yaw - Camera_Way.Yaw <= 100.f && GetActorRotation().Yaw - Camera_Way.Yaw >= -100.f && !flag)
		Aim_Yaw = UKismetMathLibrary::ClampAngle(GetActorRotation().Yaw - Camera_Way.Yaw, -100.f, 100.f);
	else
	{
		Aim_Yaw = 0;
	}
}

void AMyCharacter::DeathNotify()
{
	PauseHUDWidget->DeadPauseOn();
	AActor::Destroy();
}

void AMyCharacter::EndMontaged(UAnimMontage* Montage, bool bInterrupted)
{
	if (Play_Amsal)
	{
		Play_Amsal = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		Weapon_Component->CurrentGunSlot = TempEWeapon;
		Weapon_Component->UpdateGunAttachment();
		CameraBoom->bDoCollisionTest = true;

		if (TempEWeapon == EWeaponSlot::WeaponSlot1)
		{
			Get_Weapon = 1;
		}
		else if (TempEWeapon == EWeaponSlot::WeaponSlot2)
		{
			Get_Weapon = 2;
		}
		else if (TempEWeapon == EWeaponSlot::WeaponSlot3)
		{
			Get_Weapon = 3;
		}
	}
}

void AMyCharacter::LButtonDown()
{
	if (Dead)
		return; 

	Is_LeftButtonOn = true;
	Aiming();
}

void AMyCharacter::LButtonRelease()
{
	Is_LeftButtonOn = false;
	AimingEnd();
}
void AMyCharacter::StartShoot()
{
	isFiring = true;
}

void AMyCharacter::StopShoot()
{
	isFiring = false;
}

void AMyCharacter::StopDamage()
{
	isDamage = false;
}

void AMyCharacter::BackGroundOffSwitch()
{
	MainHUDWidget->BackGroundOffVisi();
}

void AMyCharacter::BackGround()
{
	Weapon_Component->SwapTexture();
}

void AMyCharacter::TimeLineUpdateFunc(float Output)
{
	
	if (IsValid(Actor_Train) == true)
	{
		FVector Loc1 = LeftCamera->GetComponentLocation();
		FVector Loc2 = GetActorRightVector() * Output;
		FVector Loc3;
		if(!Is_crouch)
			Loc3 = GetActorUpVector() * 80.f;
		else
			Loc3 = GetActorUpVector() * 50.f;

		FollowCamera->SetWorldLocation(Loc1 + Loc2 + Loc3);
	}
	else
	{
		LeftSpring = Output;
	}
}

void AMyCharacter::TimeLineSprintUpdateFunc(float Output)
{
	CameraBoom->TargetArmLength = Output;
}

void AMyCharacter::TimeLineCrouchArmUpdateFunc(float Output)
{
	CameraBoom->TargetArmLength = Output;
}

void AMyCharacter::TimeLineCrouchUpdateFunc(float Capsule)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(Capsule);
}

void AMyCharacter::TimeLineCrouchCameraUpdateFunc(float Camera)
{
	if (IsValid(Actor_Train) == false)
	{
		FollowCamera->SetRelativeLocation(FVector(FollowCamera->GetRelativeLocation().X, FollowCamera->GetRelativeLocation().Y, Camera));
	}
		CrouchSpring = Camera;
}

void AMyCharacter::TimeLineAimingUpdateFunc(float Output)
{
	float RightDistance = 80.f;

	if (ChangeCameraPos)
		RightDistance = RightDistance * -1;

	CameraBoom->TargetArmLength = Output;
}

void AMyCharacter::OnInteraction()
{
	
}

void AMyCharacter::Fire()
{
	if (!Is_CharacterCanFire || Dead)
		return;

	if(Is_Zoom && !Is_Throwing)
		Weapon_Component->GunFire();

	if (Is_Throwing)
	{
		Throw();
	}
}

void AMyCharacter::FireRelease()
{
	Weapon_Component->ReleaseFire();
}

void AMyCharacter::ChangeSelector()
{
	Weapon_Component->WeaponSelectorChange();
}

void AMyCharacter::ChangeSlot1()
{
	if (Weapon_Component->IsShooting || EventOn) return;

	if (Weapon_Component->ADSMode) FPP_Mode();

	if (Weapon_Component->CurrentGunSlot == EWeaponSlot::WeaponSlot1) return;

	if (!IsValid(Weapon_Component->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot1)])) return;

	Weapon_Component->ReleaseFire();

	if (Is_Throwing == true) StopThrow();

	if (Weapon_Component->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot1)] && (Weapon_Component->CurrentGunSlot != EWeaponSlot::WeaponSlot1 || !IsValid(Weapon_Component->CurrentEquipWeapon)))
	{
		PlaySlotReleaseAnimation(EWeaponSlot::WeaponSlot1);
		Get_Weapon = 1;
	}
	else if (Weapon_Component->CurrentGunSlot == EWeaponSlot::WeaponSlot1 || !IsValid(Weapon_Component->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot1)]))
	{
		Weapon_Component->SetLaserSightVisual(false);
		Is_CharacterCanFire = false;
		PlaySlotHoldAnimation(EWeaponSlot::WeaponSlot1);
		Get_Weapon = 0;
	}
	ButtonOnOff = true;
}

void AMyCharacter::ChangeSlot2()
{
	if (Weapon_Component->IsShooting || EventOn) return;

	if (Weapon_Component->ADSMode) FPP_Mode();

	if (Weapon_Component->CurrentGunSlot == EWeaponSlot::WeaponSlot2) return;

	if (!IsValid(Weapon_Component->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot2)])) return;

	Weapon_Component->ReleaseFire();

	if (Is_Throwing == true) StopThrow();

	if (Weapon_Component->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot2)] && (Weapon_Component->CurrentGunSlot != EWeaponSlot::WeaponSlot2 || !IsValid(Weapon_Component->CurrentEquipWeapon)))
	{
		PlaySlotReleaseAnimation(EWeaponSlot::WeaponSlot2);
		
		Get_Weapon = 2;
	}
	else if (Weapon_Component->CurrentGunSlot == EWeaponSlot::WeaponSlot2)
	{
		Weapon_Component->SetLaserSightVisual(false);
		Is_CharacterCanFire = false;
		PlaySlotHoldAnimation(EWeaponSlot::WeaponSlot2);
		Get_Weapon = 0;
	}
	ButtonOnOff = true;
}

void AMyCharacter::ChangeSlot3()
{
	if (Weapon_Component->IsShooting || EventOn) return;

	if (Weapon_Component->ADSMode) FPP_Mode();

	if (Weapon_Component->CurrentGunSlot == EWeaponSlot::WeaponSlot3) return;

	if (!IsValid(Weapon_Component->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot3)])) return;

	Weapon_Component->ReleaseFire();

	if (Is_Throwing == true) StopThrow();

	if (Weapon_Component->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot3)] && (Weapon_Component->CurrentGunSlot != EWeaponSlot::WeaponSlot3 || !IsValid(Weapon_Component->CurrentEquipWeapon)))
	{
		PlaySlotReleaseAnimation(EWeaponSlot::WeaponSlot3);
		Get_Weapon = 3;
	}
	else if (Weapon_Component->CurrentGunSlot == EWeaponSlot::WeaponSlot3 || !IsValid(Weapon_Component->GunSlot[static_cast<int>(EWeaponSlot::WeaponSlot3)]))
	{
		Weapon_Component->SetLaserSightVisual(false);
		Is_CharacterCanFire = false;
		PlaySlotHoldAnimation(EWeaponSlot::WeaponSlot3);
		Get_Weapon = 0;
	}
	ButtonOnOff = true;

 }

void AMyCharacter::ReloadStart()
{
	if (Dead)
		return;

	Weapon_Component->WeaponReload();
}

void AMyCharacter::AddAmmo(int _slot, int _Amount)
{
	if (Weapon_Component->Ammo.Num() > _slot && 0 <= _slot)
	{
		Weapon_Component->Ammo[_slot] += _Amount;

		if (_slot == 0)
			Weapon_Component->ClipAllNum = Weapon_Component->Ammo[_slot];
		else if (_slot == 1)
			Weapon_Component->ClipAllNum2 = Weapon_Component->Ammo[_slot];
		else if (_slot == 2)
			Weapon_Component->ClipAllNum3 = Weapon_Component->Ammo[_slot];
	}
}

void AMyCharacter::ThrowMode()
{
	bool ADSMode = Weapon_Component->ADSMode || Weapon_Component->KeepADSMode;

	if (ADSMode == false)
	{
		if (Is_Throwing == false && IsGrenadeCoolTimeReady == true)
		{
			Aiming();
			CrossHairWidget->SetCrossHairVisible(false);
			ReadytoThrow();
		}
		else
		{
			StopThrow();
		}
	}
}

void AMyCharacter::ReadytoThrow()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("Grenade Anim"));
	//PlayAnimMontage(GrenadeMontage, 1.0f);
	PlayGrenadeThrowAnimation();

	Is_Throwing = true;

	//Test
	/*ThrowObjectSpawn_Notify();
	Is_Throwing = true;
	ThrowReady_Notify();*/
}

void AMyCharacter::Throw()
{
	if (Is_Throwing)
	{
		PlayGrenadeThrowEndAnimation();
	}
}

void AMyCharacter::ThrowObject_Notify()
{
	if (Is_Throwing)
	{
		if (IsValid(SpawnThrowObject))
		{
			FVector LaunchVelocity = CalculateThrowVelocity();

			SpawnThrowObject->SetActorLocation(GetActorLocation() + FVector(0,0,90));
			SpawnThrowObject->Throw(LaunchVelocity);

			ProjectileStart->SetWorldLocation(SpawnThrowObject->GetActorLocation());

			SpawnThrowObject = nullptr;

			IsGetThrowObject = false;

			ClearSplineMesh();

			float InDelayTime = 1.f;
			FTimerHandle TH;
			GetWorld()->GetTimerManager().SetTimer(TH, FTimerDelegate::CreateLambda([&]()
				{
					if (Is_Throwing)
					{
						ReadytoThrow();
					}
				}
			), InDelayTime, false);
		}
	}
}

void AMyCharacter::StopThrow()
{
	StopGrenadeThrowAnimation();

	Is_Throwing = false;

	if (Is_LeftButtonOn==true)
	{
		CrossHairWidget->SetCrossHairVisible(true);
	}
	else
	{
		AimingEnd();
	}


	ClearSplineMesh();

	if (true == IsGetThrowObject)
	{
		//손에 수류탄 없애주기
		if (IsValid(SpawnThrowObject))
		{
			SpawnThrowObject->Destroy();
		}
		IsGetThrowObject = false;
	}
}

void AMyCharacter::StopPauseOn()
{
	if (StopPauseSwitch == false)
	{
		StopPauseHUDWidget->OptionOn();
		StopPauseSwitch = true;
	}
	else if (StopPauseSwitch == true)
	{
		StopPauseHUDWidget->OptionOff();
		StopPauseSwitch = false;
	}
}

void AMyCharacter::ShowProjectilePath()
{
	if (IsGetThrowObject)
	{
		FHitResult HitResult;
		TArray<FVector> OutPathPositions;
		FVector LastTracePosition;

		FVector StartPos = ProjectileStart->GetComponentLocation();

		FRotator Rot = GetControlRotation();
		FVector LaunchVelocity = CalculateThrowVelocity();

		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);

		ClearSplineMesh();

		if (UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), HitResult, OutPathPositions, LastTracePosition, StartPos, LaunchVelocity,
			true, 0.0f, ECollisionChannel::ECC_Visibility, false, IgnoreActors, EDrawDebugTrace::None, 10.0f))
		{
			if (IsValid(SpawnThrowObject))
			{
				AThrowable_Rock* Object = Cast<AThrowable_Rock>(SpawnThrowObject);

				if (IsValid(Object))
				{
					float UpScale = Object->SoundRadius / 50.f;

					ProjectileEndSphere->SetRelativeScale3D(FVector(UpScale, UpScale, 0.1f));
				}				
			}

			FVector EndPosition;
			FHitResult FixResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			if (GetWorld()->LineTraceSingleByChannel(FixResult, LastTracePosition - FVector(0, 0, -50), LastTracePosition, ECC_Visibility, Params)) {
				EndPosition = FixResult.Location;
			}
			else {
				EndPosition = LastTracePosition;
				EndPosition.Z += 30.f;
			}

			ProjectileEndSphere->SetWorldLocation(EndPosition);
			ProjectileEndSphere->SetVisibility(true);
		}

		for (int i = 0; i < OutPathPositions.Num(); i++)
		{
			//DrawDebugPoint(GetWorld(), OutPathPositions[i], 20.0f, FColor::Red);

			ProjectilePath->AddSplinePointAtIndex(OutPathPositions[i], i, ESplineCoordinateSpace::World);
		}

		//스플라인의 끝 제외
		int SplineCount = ProjectilePath->GetNumberOfSplinePoints() - 1;

		for (int i = 0; i < SplineCount; i++)
		{
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);
			SplineMeshComponent->SetMobility(EComponentMobility::Movable);
			SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;

			SplineMeshComponent->RegisterComponentWithWorld(GetWorld());

			SplineMeshComponent->AttachToComponent(ProjectilePath, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMeshComponent->SetStartScale(FVector2D({ 0.1f, 0.1f }));
			SplineMeshComponent->SetEndScale(FVector2D({ 0.1f, 0.1f }));

			FVector StartPoint = ProjectilePath->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			FVector StartTangent = ProjectilePath->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
			FVector EndPoint = ProjectilePath->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
			FVector EndTangent = ProjectilePath->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
			SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);

			if (i == 0)//시작 스플라인에 지정
			{
			}
			else
			{
				if (IsValid(ProjectileDefaultMesh))
				{
					SplineMeshComponent->SetStaticMesh(ProjectileDefaultMesh);
				}

				if (ProjectilePathMaterial)
				{
					SplineMeshComponent->SetMaterial(0, ProjectilePathMaterial);
				}
			}

			Spline_Meshes.Add(SplineMeshComponent);
		}

		float InDelayTime = GetWorld()->DeltaTimeSeconds;
		FTimerHandle TH;
		GetWorld()->GetTimerManager().SetTimer(TH, FTimerDelegate::CreateLambda([&]()
			{
				ShowProjectilePath();
			}
		), InDelayTime, false);
	}
}

void AMyCharacter::ClearSplineMesh()
{
	ProjectileEndSphere->SetVisibility(false);

	ProjectilePath->ClearSplinePoints();

	int SplineCount = Spline_Meshes.Num();

	for (int i = 0; i < SplineCount; i++)
	{
		Spline_Meshes[i]->DestroyComponent();
	}

	Spline_Meshes.Empty();
}

void AMyCharacter::ClearGrenadeCoolTime()
{
	IsGrenadeCoolTimeReady = false;
	CurGrenadeCoolTime = 0.f;
	MainHUDWidget->SetGrenadeCoolTime(0.f);
}

FVector AMyCharacter::CalculateThrowVelocity()
{
	return (UKismetMathLibrary::GetForwardVector(GetControlRotation()) + FVector(0.f, 0.f, 0.4f)) * ThrowPower;
}

void AMyCharacter::ThrowObjectSpawn_Notify()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("GrenadeSpawn_AnimNotify"));

	FTransform Transform;

	//Transform.SetLocation(GetMesh()->GetSocketLocation("middle_01_l"));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (IsValid(ThrowObject))
	{
		SpawnThrowObject = GetWorld()->SpawnActor<AThrowableObject>(ThrowObject, Transform, SpawnParams);
		SpawnThrowObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("middle_01_l"));
		if (IsValid(Actor_Train) == true) {
			SpawnThrowObject->Actor_Train = Actor_Train;
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("ThrowObject is Not Vaild"));
	}

	IsGetThrowObject = true;
}

void AMyCharacter::ThrowReady_Notify()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("ThrowReady_AnimNotify"));

	ShowProjectilePath();

}

void AMyCharacter::SetActorFlag(AActor* Loc1)
{
	Flag = Loc1;
	PlayerLoc = GetActorLocation();
	Player_FlagMontage();
}

void AMyCharacter::PlayReloadAnimation()
{
	K2_PlayReloadAnimation();
}

void AMyCharacter::PlayFireAnimation()
{
	K2_PlayFireAnimation();
}

void AMyCharacter::PlaySlotHoldAnimation(EWeaponSlot _Slot)
{
	K2_PlaySlotHoldAnimation(_Slot);
}

void AMyCharacter::PlaySlotReleaseAnimation(EWeaponSlot _Slot)
{
	K2_PlaySlotReleaseAnimation(_Slot);
}

void AMyCharacter::PlayGrenadeThrowAnimation()
{
	K2_PlayGrenadeThrowAnimation();
}

void AMyCharacter::PlayGrenadeThrowEndAnimation()
{
	K2_PlayGrenadeThrowEndAnimation();
}

void AMyCharacter::StopGrenadeThrowAnimation()
{
	K2_StopGrenadeThrowAnimation();
}

void AMyCharacter::PlayOwnerFireAnimation(EWeaponType _Type)
{
	K2_PlayOwnerFireAnimation(_Type);
}

void AMyCharacter::TriggerEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		IInteractInterface::Execute_OnInteractRangeEntered(OtherActor, this);
	}
}

void AMyCharacter::TriggerExit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		IInteractInterface::Execute_OnInteractRangeExited(OtherActor, this);
	}
}

void AMyCharacter::FPP_Mode()
{
	AGunBase* CurrentWeapon = Weapon_Component->CurrentEquipWeapon;
	bool ADSMode = Weapon_Component->ADSMode;
	float BlendTime = 0.1f;
	float BlendExp = 0.4f;

	if (Is_Throwing == true) return;

	if (Is_Zoom==false) return;

	if (IsValid(CurrentWeapon))
	{
		EWeaponType WeaponType = CurrentWeapon->WeaponType;
		AADS_Camera* ADS_Camera = CurrentWeapon->GetAdsCamera();

		ADS_Camera->GetCamera()->SetFieldOfView(80.f);
		
		if (ADSMode == false)
		{
			if (Weapon_Component->IsReloading == true) return;

			Weapon_Component->ADSMode = true;

			CurrentWeapon->UpdateAdsCameraPosition();
			GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(ADS_Camera, BlendTime, EViewTargetBlendFunction::VTBlend_Cubic, BlendExp);
			Weapon_Component->SetAimPointCamera(ADS_Camera->GetCamera());

			CrossHairWidget->SetCrossHairVisible(false);

			FTimerHandle TH;
			float InDelayTime = BlendTime;
			GetWorld()->GetTimerManager().SetTimer(TH, FTimerDelegate::CreateLambda([&]()
				{
					if (Weapon_Component->ADSMode == true)
					{
						Head->SetVisibility(false);
					}
				}
			), InDelayTime, false);



			if (WeaponType == EWeaponType::SniperRifle)
			{
				AOpticsBase* temp = CurrentWeapon->IsAttachmentExist<AOpticsBase>(EAttachmentType::Optics);

				if (IsValid(temp))
				{
					SniperWidget->SetVisibility(ESlateVisibility::Visible);
					GetMesh()->HideBoneByName("lowerarm_l", EPhysBodyOp::PBO_None);
					GetMesh()->HideBoneByName("lowerarm_r", EPhysBodyOp::PBO_None);
					GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(1.f, 0.f, 1.5f, FLinearColor::Black, true, false);
					CurrentWeapon->HideGun(false);
				}
			}
		}
		else
		{
			Weapon_Component->ADSMode = false;

			LeftCamera->SetActive(false);

			GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, BlendTime, EViewTargetBlendFunction::VTBlend_Cubic, BlendExp);
			LeftCamera->SetActive(true);

			Head->SetVisibility(true);
			CrossHairWidget->SetCrossHairVisible(true);

			Weapon_Component->SetAimPointCamera(FollowCamera);

			if (WeaponType == EWeaponType::SniperRifle)
			{
				AOpticsBase* temp = CurrentWeapon->IsAttachmentExist<AOpticsBase>(EAttachmentType::Optics);

				if (IsValid(temp))
				{
					SniperWidget->SetVisibility(ESlateVisibility::Collapsed);
					GetMesh()->UnHideBoneByName("lowerarm_l");
					GetMesh()->UnHideBoneByName("lowerarm_r");
					GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(1.f, 0.f, 1.5f, FLinearColor::Black, true, false);
					CurrentWeapon->HideGun(true);
				}
			}
		}
	}
}

void AMyCharacter::DecisionOffTime()
{
	ButtonOnOff = false;
}

void AMyCharacter::WheelEvent(float Value)
{
	bool ADSMode = Weapon_Component->ADSMode;

	if (Is_Throwing)
	{
		ThrowPower = FMath::Clamp((ThrowPower + (50.f * Value)), 500.f, 1500.f);
	}

	if (ADSMode)
	{
		AGunBase* CurrentWeapon = Weapon_Component->CurrentEquipWeapon;

		if (IsValid(CurrentWeapon))
		{
			EWeaponType WeaponType = CurrentWeapon->WeaponType;

			if (WeaponType == EWeaponType::SniperRifle)
			{
				AOpticsBase* temp = CurrentWeapon->IsAttachmentExist<AOpticsBase>(EAttachmentType::Optics);
				AADS_Camera* ADS_Camera = CurrentWeapon->GetAdsCamera();

				if (IsValid(temp))
				{
					float FOV = temp->SniperFOV;
					float ZoomScale = FMath::Clamp((FOV + (5.f * Value)), 20.f, 80.f);

					ADS_Camera->GetCamera()->SetFieldOfView(ZoomScale);
					temp->SniperFOV = ZoomScale;
				}
			}
		}
	}
}

bool AMyCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const {
	static const FName NAME_AILineOfSight = FName(TEXT("TestPawnLineOfSight"));
	FHitResult HitResult;
	FVector View_Location = OutSeenLocation;
	auto sockets = GetMesh()->GetAllSocketNames();
	int View_Size = 0;
	bool View_Temp = false;

	if (IsValid(Actor_Train) == false) {
		for (int i = 0; i < sockets.Num(); i += 1) {
			if (sockets[i] == FName("neck_01") || sockets[i] == FName("hand_l") || sockets[i] == FName("hand_r") ||
				sockets[i] == FName("pelvis") || sockets[i] == FName("ik_foot_root")) {
				FVector socketLocation = GetMesh()->GetSocketLocation(sockets[i]);
	
				// const bool bHitSocket = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, socketLocation
				// 	, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
				// 	, FCollisionQueryParams(NAME_AILineOfSight, false, IgnoreActor));
				bool bHitSocket = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, socketLocation,
					ECC_GameTraceChannel3, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));	
			
				NumberOfLoSChecksPerformed += 1;
			
				if (bHitSocket == false || (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsOwnedBy(this))) {
					if (sockets[i] == FName("neck_01") || sockets[i] == FName("hand_l") || sockets[i] == FName("hand_r")) {
						if (View_Temp == false) {
							View_Temp = true;
							View_Size += 1;
							View_Location = socketLocation;
							//DrawDebugSphere(GetWorld(), View_Location, 20, 5, FColor::Red, false, 5, 0, 3);
						}
					}
					else if (sockets[i] == FName("pelvis") || sockets[i] == FName("ik_foot_root")) {
						View_Size += 1;
						View_Location = socketLocation;
						//DrawDebugSphere(GetWorld(), View_Location, 20, 5, FColor::Red, false, 5, 0, 3);
					}
				}
				}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%d"), View_Size));
		auto AI = Cast<AMyCharacterAI>(IgnoreActor);
		if (IsValid(AI) == true) {
			if (IsValid(AI->AI_Controller) == true) {
				AI->AI_Controller->ViewCount = View_Size;
			}
		}

		if (View_Size > 0) {
			OutSeenLocation = View_Location;
			OutSightStrength = 1;
			return true;
		}

	
		// const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, GetActorLocation()
		// 	, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
		// 	, FCollisionQueryParams(NAME_AILineOfSight, false, IgnoreActor));
		//
		// NumberOfLoSChecksPerformed += 1;
		//
		// if (bHit == false || (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsOwnedBy(this))) {
		// 	OutSeenLocation = GetActorLocation();
		// 	OutSightStrength = 1;
		//
		// 	return true;
		// }
	
		OutSightStrength = 0;
		return false;
	}
	else {
		FVector SightTargetLocation = GetActorLocation() - (TrainCapsule->GetComponentLocation() - GetMesh()->GetSocketLocation("neck_01"));
		// bool bRayHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, SightTargetLocation
		// 		, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_GameTraceChannel3))
		// 		, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));
		
		bool bRayHit = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, SightTargetLocation, ECC_GameTraceChannel3, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));	
		if (bRayHit == false || (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsOwnedBy(this))) {
			OutSeenLocation = SightTargetLocation;
			OutSightStrength = 1;
			return true;
		}
		OutSightStrength = 0;
		return false;
	}
}

void AMyCharacter::UpdateVisionState(const bool& NewState)
{
	NightVisionState = NewState;
}

bool AMyCharacter::IsVisionStateModified(const bool& NewState) const
{
	return NightVisionState != NewState;
}

bool AMyCharacter::GetNightVisionState()
{
	return NightVisionState;
}

void AMyCharacter::LoadData()
{
	UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	TSharedPtr<FJsonObject> json = inst->FindJsonByTag(this->Tags.IsEmpty() ? FName("") : this->Tags[0]);
	if (nullptr == json)
	{
		UE_LOG(LogTemp, Log, TEXT("Can not find saved data for %s"), *GetName());
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Found saved data for %s"), *GetName());
	FPlayerSaveData data;
	data.SetDeserializedData(json, this);
	return;
}

void AMyCharacter::SetSavedLocation(const FVector& SaveLocation)
{
	this->LastSavedLocation = SaveLocation;
}

FVector AMyCharacter::GetSavedLocation()
{
	return LastSavedLocation;
}