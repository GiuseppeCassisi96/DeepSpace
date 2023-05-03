// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlfredFSM.h"


UAlfredFSM::UAlfredFSM()
{
	CalmState = NewObject<UFSMState>();
	HearingState = NewObject<UFSMState>();
	WarningState = NewObject<UFSMState>();
	AttackState = NewObject<UFSMState>();
	AllyState = NewObject<UFSMState>();

	AttackBT = NewObject<UAttackBT>();
	CalmBT = NewObject<UCalmBT>();

	States.Add(EEnemyState::Calm, CalmState);
	States.Add(EEnemyState::Hearing, HearingState);
	States.Add(EEnemyState::Warning, WarningState);
	States.Add(EEnemyState::Attack, AttackState);
	States.Add(EEnemyState::Ally, AllyState);

	States[EEnemyState::Calm]->Action.BindUFunction(CalmBT, "RunTree");

	InitialState = EEnemyState::Calm;
	CurrentState = InitialState;
}

TMap<EEnemyState, UFSMState*>& UAlfredFSM::GetStates()
{
	return States;
}


int UAlfredFSM::RunActionOfCurrentState()
{
	try
	{
		return States[CurrentState]->RunAction();
	}
	catch (ActionFailException e)
	{
		std::cout <<  e.what() << "\n";
		return -1;
	}
	
}

