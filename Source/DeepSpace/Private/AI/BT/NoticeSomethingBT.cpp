// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/NoticeSomethingBT.h"

#include "NavigationPath.h"

void UNoticeSomethingBT::InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys, FVector location)
{
	Super::InitTree(owner, navSys);
	//INSTANCIES CREATION PHASE:
		//TASKS
	TaskCanReach = NewObject<UTaskBT>(this, FName("FTNoticeSomethingBT"));
	TaskGoToLocation = NewObject<UTaskBT>(this, FName("STNoticeSomethingBT"));
	//COMPOSITE TASKS
	FirstSequence = NewObject<USequenceBT>(this, FName("FSNoticeSomethingBT"));

	//BINDING PHASE:
	TaskCanReach->Task.BindUFunction(this, TEXT("CanReach"));
	TaskGoToLocation->Task.BindUFunction(this, TEXT("GoToLocation"));

	//ADDING PHASE:
	FirstSequence->Tasks.Add(TaskCanReach);
	FirstSequence->Tasks.Add(TaskGoToLocation);

	//PARAM SETTING PHASE:
	RootTask = FirstSequence;
	SourceLocation = location;
	bIsStopped = false;
}

ETaskExeState UNoticeSomethingBT::RunTree()
{
	if (!bIsStopped)
	{
		TreeExeState = RootTask->RunTask();
		return TreeExeState;
	}
	TreeExeState = ETaskExeState::Stopped;
	return ETaskExeState::Stopped;
}

void UNoticeSomethingBT::StopTree()
{
	Super::StopTree();
}

ETaskExeState UNoticeSomethingBT::CanReach()
{
	if(NavSys->FindPathToLocationSynchronously(ownerBT->GetWorld(),
		ownerBT->GetActorLocation(),SourceLocation)->IsValid())
	{
		return ETaskExeState::Success;
	}
	return ETaskExeState::Fail;
}

ETaskExeState UNoticeSomethingBT::GoToLocation()
{
	AIController->MoveToLocation(SourceLocation);
	return ETaskExeState::Success;
}
