// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlfredFSM.h"

#include <iostream>

#include "Engine/Engine.h"
#include "GameFramework/Actor.h"

UAlfredFSM::UAlfredFSM()
{
	CalmState = NewObject<UFSMState>();
	HearingState = NewObject<UFSMState>();
	WarningState = NewObject<UFSMState>();
	AttackState = NewObject<UFSMState>();
	AllyState = NewObject<UFSMState>();

	States.Add(EEnemyState::Calm, CalmState);
	States.Add(EEnemyState::Hearing, HearingState);
	States.Add(EEnemyState::Warning, WarningState);
	States.Add(EEnemyState::Attack, AttackState);
	States.Add(EEnemyState::Ally, AllyState);

	InitialState = EEnemyState::Calm;
	CurrentState = InitialState;
	//RunActionOfCurrentState();
}

TMap<EEnemyState, UFSMState*>& UAlfredFSM::GetStates()
{
	return States;
}


bool UAlfredFSM::RunActionOfCurrentState()
{
	try
	{
		return States[CurrentState]->RunAction();
	}
	catch (ActionFailException e)
	{
		std::cout <<  e.what() << "\n";
		return false;
	}
	
}
