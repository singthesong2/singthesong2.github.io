// Fill out your copyright notice in the Description page of Project Settings.

#include "Characte_AI_Ani.h"
#include "MyCharacter.h"

UCharacte_AI_Ani::UCharacte_AI_Ani()
{

}

void UCharacte_AI_Ani::NativeUpdateAnimation(float DeltaSeconds)
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
		}
	}
}