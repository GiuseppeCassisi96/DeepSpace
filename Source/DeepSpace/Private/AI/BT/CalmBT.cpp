// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/CalmBT.h"


ETaskExeState UCalmBT::RunTree()
{
	if (!bIsStopped)
	{
		TreeExeState = RootTask->RunTask();
		return TreeExeState;
	}
	TreeExeState = ETaskExeState::Stopped;
	return ETaskExeState::Stopped;
}




void UCalmBT::InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys)
{
	Super::InitTree(owner, navSys);
	//INSTANCIES CREATION PHASE:
		//TASKS
	TaskIsReachable = NewObject<UTaskBT>(this, FName("FTCalmBT"));
	TaskGoToRandPosition = NewObject<UTaskBT>(this, FName("STCalmBT"));
	//COMPOSITE TASKS
	FirstSequence = NewObject<USequenceBT>(this, FName("FSCalmBT"));
	//DECORATOR
	CalmUntilFail = NewObject<UUntilFailBT>(this, FName("UFCalmBT"));

	//BINDING PHASE:
	TaskIsReachable->Task.BindUFunction(this, TEXT("IsReachable"));
	TaskGoToRandPosition->Task.BindUFunction(this, TEXT("GoToRandPosition"));

	//ADDING PHASE:
	FirstSequence->Tasks.Add(TaskIsReachable);
	FirstSequence->Tasks.Add(TaskGoToRandPosition);

	//PARAM SETTING PHASE:
	CalmUntilFail->childTask = FirstSequence;
	RootTask = CalmUntilFail;
	bIsStopped = false;
}


void UCalmBT::StopTree()
{
	Super::StopTree();
	ownerBT->GetWorldTimerManager().ClearTimer(TimerHandle);
}

//Condition
ETaskExeState UCalmBT::IsReachable()
{
	if (NavSys->GetRandomReachablePointInRadius(ownerBT->GetActorLocation(),
		3000.0f, randLocation))
	{
		return ETaskExeState::Success;//Success!
	}
	return ETaskExeState::Fail;//Fail !
}

//Action
ETaskExeState UCalmBT::GoToRandPosition()
{
	AIController->MoveToLocation(randLocation.Location);
	//I create a timer to implement until fail decorator
	ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this,
		FTimerDelegate::TMethodPtr<UCalmBT>(&UCalmBT::RunTree),
		10.0f, false);
	return ETaskExeState::Success;//Success !
}







