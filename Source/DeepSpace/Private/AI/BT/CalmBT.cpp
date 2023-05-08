// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/CalmBT.h"




UCalmBT::UCalmBT()
{
	firstSequence = NewObject<USequenceBT>();
	firstTask = NewObject<UTaskBT>();
	secondTask = NewObject<UTaskBT>();
	CalmUntilFail = NewObject<UUntilFailBT>();
	CalmUntilFail->childTask = firstSequence;
	RootTask = CalmUntilFail;

	firstTask->BindTask(this, TEXT("IsReachable"));
	secondTask->BindTask(this, TEXT("GoToRandPosition"));

	firstSequence->Tasks.Add(firstTask);
	firstSequence->Tasks.Add(secondTask);

	TimerDelegate.BindUFunction(this, TEXT("ExeTreeInTimeIntervall"));
	bIsStopped = false;
}

//Condition
int UCalmBT::IsReachable()
{
	UE_LOG(LogTemp, Warning, TEXT("UCalmBT::IsReachable"))
	if (NavSys->GetRandomReachablePointInRadius(ownerBT->GetActorLocation(),
		3000.0f, randLocation))
	{
		TreeExeCode = 0;
		return 0;//Success!
	}
	TreeExeCode = -1;
	return -1;//Fail !
}

//Action
int UCalmBT::GoToRandPosition()
{
	UE_LOG(LogTemp, Warning, TEXT("UCalmBT::GoToRandPosition"))
	AIController->MoveToLocation(randLocation.Location);
	TreeExeCode = 0;
	return 0;//Success !
}


int UCalmBT::RunTree()
{
	if(!bIsStopped)
	{
		UE_LOG(LogTemp, Warning, TEXT("RUN"));
		TreeExeCode = RootTask->RunTask();
		ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this,
			FTimerDelegate::TMethodPtr<UCalmBT>(&UCalmBT::ExeTreeInTimeIntervall),
			10.0f, false);
		return 0;
	}
	return -1;
}

void UCalmBT::InitTree(ACharacter* owner, UNavigationSystemV1* navSys)
{
	Super::InitTree(owner, navSys);
	AIController->ReceiveMoveCompleted.AddDynamic(this, &UCalmBT::ResetTimer);
}

int UCalmBT::ExeTreeInTimeIntervall()
{
	if(RootTask->RunTask() != -1)
	{
		TreeExeCode = 1;
		return 1;
	}
	TreeExeCode = 0;
	return 0;
}

void UCalmBT::ResetTimer(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	ownerBT->GetWorldTimerManager().ClearTimer(TimerHandle);
	if(!bIsStopped && Result == EPathFollowingResult::Success)
	{
		ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this,
			FTimerDelegate::TMethodPtr<UCalmBT>(&UCalmBT::ExeTreeInTimeIntervall),
			10.0f, false);
	}
}

void UCalmBT::StopTree()
{
	Super::StopTree();
	ownerBT->GetWorldTimerManager().ClearTimer(TimerHandle);
}


