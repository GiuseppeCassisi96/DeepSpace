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
	return States[CurrentState]->RunTree();
}

void UAlfredFSM::StopAction()
{
	States[CurrentState]->StopTree();
}

void UAlfredFSM::CheckTreeStatus()
{
	if(States[CurrentState]->TreeExeState == ETaskExeState::Fail)
	{
		GoToNewState(EEnemyState::Calm);
		RunActionOfCurrentState();
	}
}


