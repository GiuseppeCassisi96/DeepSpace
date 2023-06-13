// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlfredFSM.h"


UAlfredFSM::UAlfredFSM()
{
	//Set the default state
	InitialState = EEnemyState::Calm;
	CurrentState = InitialState;
}

TMap<EEnemyState, UBTInterface*>& UAlfredFSM::GetStates()
{
	return States;
}


ETaskExeState UAlfredFSM::RunActionOfCurrentState()
{
	//I reset the boolean var for sure
	States[CurrentState]->bIsStopped = false;
	ETaskExeState state = States[CurrentState]->RunTree();
	if(state == ETaskExeState::Fail)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f,
			FColor::Red, TEXT("FAIL"));
		GoToNewState(EEnemyState::Calm);
		RunActionOfCurrentState();
	}
	return state;
}




