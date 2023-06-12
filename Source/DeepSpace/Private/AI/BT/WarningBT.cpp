// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/WarningBT.h"

#include "NavigationPath.h"


ETaskExeState UWarningBT::RunTree()
{
	if (!bIsStopped)
	{
		TreeExeState = RootTask->RunTask();
		return TreeExeState;
	}
	TreeExeState = ETaskExeState::Stopped;
	return ETaskExeState::Stopped;
}

void UWarningBT::StopTree()
{
	Super::StopTree();
	ownerBT->GetWorldTimerManager().ClearTimer(TimerHandle);
}

void UWarningBT::InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys)
{
	Super::InitTree(owner, navSys);
	//INSTANCIES CREATION PHASE:
		//TASKS
	TaskcanReachRandPos = NewObject<UTaskBT>(this, FName("FTWarningBT"));
	TaskGoToRandPos = NewObject<UTaskBT>(this, FName("STWarningBT"));
	//COMPOSITE TASKS
	FirstSequence = NewObject<USequenceBT>(this, FName("FSWarningBT"));
	//DECORATOR
	CalmUntilFail = NewObject<UUntilFailBT>(this, FName("UFWarningBT"));

	//BINDING PHASE:
	TaskcanReachRandPos->Task.BindUFunction(this, TEXT("CanReachRandPos"));
	TaskGoToRandPos->Task.BindUFunction(this, TEXT("GoToRandPos"));

	//ADDING PHASE:
	FirstSequence->Tasks.Add(TaskcanReachRandPos);
	FirstSequence->Tasks.Add(TaskGoToRandPos);

	//PARAM SETTING PHASE:
	CalmUntilFail->childTask = FirstSequence;
	RootTask = CalmUntilFail;
	bIsStopped = false;
}
//Condition
ETaskExeState UWarningBT::CanReachRandPos()
{
	if (NavSys->GetRandomReachablePointInRadius(Location,
		1800.0f, randLocation))
	{
		return ETaskExeState::Success;//Success!
	}
	return ETaskExeState::Fail;//Fail !
}
//Action
ETaskExeState UWarningBT::GoToRandPos()
{
	AIController->MoveToLocation(randLocation.Location);
	//I create a timer to implement until fail decorator
	ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this,
		FTimerDelegate::TMethodPtr<UWarningBT>(&UWarningBT::RunTree),
		10.0f, false);
	return ETaskExeState::Success;//Success !
}

