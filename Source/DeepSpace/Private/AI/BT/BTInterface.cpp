// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTInterface.h"


UBTInterface::UBTInterface()
{
	RootTask = NewObject<UTaskBT>();
}

ETaskExeState UBTInterface::RunTree()
{
	return RootTask->RunTask();
}

void UBTInterface::StopTree()
{
	bIsStopped = true;
}

void UBTInterface::InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys)
{
	ownerBT = owner;
	NavSys = navSys;
	AIController = Cast<AAIController>(ownerBT->GetController());
}


