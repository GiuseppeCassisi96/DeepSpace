// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/AlfredBTManager.h"

void UAlfredBTManager::StopTreeOfAState(EEnemyState TreeState)
{
	UE_LOG(LogTemp, Warning, TEXT("STOP"));
	BehaviorForest[TreeState]->StopTree();
}

// Sets default values for this component's properties
UAlfredBTManager::UAlfredBTManager()
{
	AttackBT = NewObject<UAttackBT>();
	CalmBT = NewObject<UCalmBT>();

	BehaviorForest.Add(EEnemyState::Calm, CalmBT);
	BehaviorForest.Add(EEnemyState::Attack, AttackBT);
}


// Called when the game starts
void UAlfredBTManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


