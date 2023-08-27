// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlfredFSM.h"


UAlfredFSM::UAlfredFSM()
{
	PrimaryComponentTick.bCanEverTick = false;
	//Set the default state
	InitialState = EEnemyState::Calm;
	CurrentState = InitialState;

	
}

void UAlfredFSM::AddStates(TArray<UBTInterface*> BT)
{
	//Alle states can go to default state (calm state)
	UAlfredFSMState* CalmState = NewObject<UAlfredFSMState>();
	CalmState->stateTransitions.Add(EEnemyState::Attack);
	CalmState->stateTransitions.Add(EEnemyState::NoticeSomething);
	UAlfredFSMState* WarningState = NewObject<UAlfredFSMState>();
	WarningState->stateTransitions.Add(EEnemyState::Attack);
	WarningState->stateTransitions.Add(EEnemyState::NoticeSomething);
	WarningState->stateTransitions.Add(EEnemyState::Calm);
	UAlfredFSMState* AttackState = NewObject<UAlfredFSMState>();
	AttackState->stateTransitions.Add(EEnemyState::Calm);
	AttackState->stateTransitions.Add(EEnemyState::Warning);
	UAlfredFSMState* NoticeSomethingState = NewObject<UAlfredFSMState>();
	NoticeSomethingState->stateTransitions.Add(EEnemyState::Attack);
	NoticeSomethingState->stateTransitions.Add(EEnemyState::Warning);
	NoticeSomethingState->stateTransitions.Add(EEnemyState::Calm);

	CalmState->stateAction = BT[0];
	WarningState->stateAction = BT[1];
	AttackState->stateAction = BT[2];
	NoticeSomethingState->stateAction = BT[3];

	TArray<UAlfredFSMState*> statesList;
	statesList.Add(CalmState);
	statesList.Add(WarningState);
	statesList.Add(AttackState);
	statesList.Add(NoticeSomethingState);

	for(int i = 0; i < BT.Num(); i++)
	{
		FSM.Add(static_cast<EEnemyState>(i), statesList[i]);
	}
}





