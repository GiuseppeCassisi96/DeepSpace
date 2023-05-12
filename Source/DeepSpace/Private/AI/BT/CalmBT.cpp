// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/CalmBT.h"




UCalmBT::UCalmBT()
{
	

	UE_LOG(LogTemp, Warning, TEXT("UCalmBT::UCalmBT()"));
	
}

//Condition
ETaskExeState UCalmBT::IsReachable()
{
	if (NavSys->GetRandomReachablePointInRadius(ownerBT->GetActorLocation(),
		3000.0f, randLocation))
	{
		TreeExeState = ETaskExeState::Success;
		return ETaskExeState::Success;//Success!
	}
	TreeExeState = ETaskExeState::Fail;
	return ETaskExeState::Fail;//Fail !
}

//Action
ETaskExeState UCalmBT::GoToRandPosition()
{
	AIController->MoveToLocation(randLocation.Location);
	ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this,
		FTimerDelegate::TMethodPtr<UCalmBT>(&UCalmBT::ExeTreeInTimeIntervall),
		10.0f, false);
	TreeExeState = ETaskExeState::Success;
	return ETaskExeState::Success;//Success !
}


ETaskExeState UCalmBT::ExeTreeInTimeIntervall()
{
	if (RootTask->RunTask() != ETaskExeState::Fail)
	{
		TreeExeState = ETaskExeState::TryAgain;
		return ETaskExeState::TryAgain;
	}
	TreeExeState = ETaskExeState::Success;
	return ETaskExeState::Success;
}


ETaskExeState UCalmBT::RunTree()
{
	if(!bIsStopped)
	{
		TreeExeState = RootTask->RunTask();
		return TreeExeState;
	}
	return ETaskExeState::Fail;
}




void UCalmBT::InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys)
{
	Super::InitTree(owner, navSys);
	ownerBT = owner;
	firstSequence = NewObject<USequenceBT>(this, FName("FSCalmBT"));
	firstTask = NewObject<UTaskBT>(this, FName("FTCalmBT"));
	secondTask = NewObject<UTaskBT>(this, FName("STCalmBT"));
	CalmUntilFail = NewObject<UUntilFailBT>(this, FName("UFCalmBT"));
	firstSequence->Tasks.Add(firstTask);
	firstSequence->Tasks.Add(secondTask);
	bIsStopped = false;
	firstTask->Task.BindUFunction(this, TEXT("IsReachable"));
	secondTask->Task.BindUFunction(this, TEXT("GoToRandPosition"));
	TimerDelegate.BindUFunction(this, TEXT("ExeTreeInTimeIntervall"));
	CalmUntilFail->childTask = firstSequence;
	RootTask = CalmUntilFail;
}


void UCalmBT::StopTree()
{
	Super::StopTree();
	ownerBT->GetWorldTimerManager().ClearTimer(TimerHandle);
}


