// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlfredFSM.h"


UAlfredFSM::UAlfredFSM()
{
	InitialState = EEnemyState::Calm;
	CurrentState = InitialState;
}

TMap<EEnemyState, UBTInterface*>& UAlfredFSM::GetStates()
{
	return States;
}


ETaskExeState UAlfredFSM::RunActionOfCurrentState()
{
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

void UAlfredFSM::StopAction()
{
	States[CurrentState]->StopTree();
}



