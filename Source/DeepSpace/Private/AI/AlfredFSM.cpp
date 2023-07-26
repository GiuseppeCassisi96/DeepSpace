// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlfredFSM.h"


UAlfredFSM::UAlfredFSM()
{
	//Set the default state
	InitialState = EEnemyState::Calm;
	CurrentState = InitialState;
}

void UAlfredFSM::AddStates(TArray<UBTInterface*> BT)
{
	for(int i = 0; i < BT.Num(); i++)
	{
		States.Add(static_cast<EEnemyState>(i), BT[i]);
	}
}





