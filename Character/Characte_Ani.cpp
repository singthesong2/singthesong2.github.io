// Fill out your copyright notice in the Description page of Project Settings.

#include "Characte_Ani.h"
#include "../WeaponComp/MyAniTable.h"
#include "MyCharacter.h"

UCharacte_Ani::UCharacte_Ani()
{
	
}

void UCharacte_Ani::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto MyPawn = TryGetPawnOwner();
	auto Player = Cast<AMyCharacter>(MyPawn);

	if (IsValid(MyPawn))
	{
		if (Player)
		{
			Is_Crouch = Player->Get_crocuhState();
			Is_Sprint = Player->Get_SprintState();
			Forward = Player->Get_Forward();
			Left = Player->Get_Left();
			Player_Yaw = Player->Get_Yaw();
			Player_Pitch = Player->Get_Pitch();
			MoveDirection = Player->Get_Direction();
			AniTable = Player->Get_Table();
		}
	}
}

void UCharacte_Ani::PlayMontage(FName arg)
{
	Montage_Play(AniTable->FindRow<FDataName>(arg, TEXT(""))->Montage, 1.0f);
}

void UCharacte_Ani::AnimNotify_JumpFlag()
{
	auto MyPawn = TryGetPawnOwner();
	auto Player = Cast<AMyCharacter>(MyPawn);

	Player->Noti_FlagMontage();
}

void UCharacte_Ani::AnimNotify_Death()
{
	auto MyPawn = TryGetPawnOwner();
	auto Player = Cast<AMyCharacter>(MyPawn);

	Player->DeathNotify();
}