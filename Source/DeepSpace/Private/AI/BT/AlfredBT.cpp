// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/AlfredBT.h"

UAlfredBT::UAlfredBT()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAlfredBT::InitBehaviorTrees( TObjectPtr<ACharacter> owner, 
	TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<AAlfredAIController> AlfredAIController)
{
	CalmBT = NewObject<UCalmBT>();
	WarningBT = NewObject<UWarningBT>();
	AttackBT = NewObject<UAttackBT>();
	NoticeSomethingBT = NewObject<UNoticeSomethingBT>();
	trees.Add(CalmBT);
	trees.Add(WarningBT);
	trees.Add(AttackBT);
	trees.Add(NoticeSomethingBT);
	for(int i = 0; i < trees.Num(); i++)
	{
		trees[i]->InitTree(owner, navSys, AlfredAIController);
	}
}

