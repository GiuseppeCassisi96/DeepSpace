// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/AlfredBT.h"

/*
 * 0-> Success
 * 1-> Call me again
 * -1-> Fail
 */

int UAlfredBT::RunTree()
{
	return RootTask->RunTask();
}

void UAlfredBT::StopTree()
{
	bIsStopped = true;
}

void UAlfredBT::InitTree(ACharacter* owner, UNavigationSystemV1* navSys)
{
	ownerBT = owner;
	NavSys = navSys;
	AIController = Cast<AAIController>(ownerBT->GetController());
}
