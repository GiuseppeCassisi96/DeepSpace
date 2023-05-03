// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/CalmBT.h"

/*
 * 0-> Success
 * 1-> Call me again
 * -1-> Fail
 */


UCalmBT::UCalmBT()
{
	firstSequence = NewObject<USequenceBT>();
	firstTask = NewObject<UTaskBT>();
	secondTask = NewObject<UTaskBT>();
	CalmUntilFail = NewObject<UUntilFailBT>();
	CalmUntilFail->childTask = firstSequence;
	RootTask = CalmUntilFail;

	firstTask->Task.BindUFunction(this, TEXT("IsReachable"));
	secondTask->Task.BindUFunction(this, TEXT("GoToRandPosition"));

	firstSequence->Tasks.Add(firstTask);
	firstSequence->Tasks.Add(secondTask);

	TimerDelegate.BindUFunction(this, TEXT("ExeTreeInTimeIntervall"));
	
}

//Condition
int UCalmBT::IsReachable()
{
	UE_LOG(LogTemp, Warning, TEXT("UCalmBT::IsReachable"))
	if (NavSys->GetRandomReachablePointInRadius(ownerBT->GetActorLocation(),
		3000.0f, randLocation))
	{
		return 0;//Success!
	}
	return -1;//Fail !
}

//Action
int UCalmBT::GoToRandPosition()
{
	UE_LOG(LogTemp, Warning, TEXT("UCalmBT::GoToRandPosition"))
	AIController->MoveToLocation(randLocation.Location);
	return 0;//Success !
}


int UCalmBT::RunTree()
{
	TreeExeCode = RootTask->RunTask();
	ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this, 
		FTimerDelegate::TMethodPtr<UCalmBT>(&UCalmBT::ExeTreeInTimeIntervall), 
		10.0f, false);
	return 0;
}

void UCalmBT::InitTree(ACharacter* owner, UNavigationSystemV1* navSys)
{
	ownerBT = owner;
	NavSys = navSys;
	AIController = Cast<AAIController>(ownerBT->GetController());
	AIController->ReceiveMoveCompleted.AddDynamic(this, &UCalmBT::ResetTimer);
}

int UCalmBT::ExeTreeInTimeIntervall()
{
	if(RootTask->RunTask() != -1)
	{
		return 1;
	}
	return 0;
}

void UCalmBT::ResetTimer(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	ownerBT->GetWorldTimerManager().ClearTimer(TimerHandle);
	if(TreeExeCode != -1 && Result == EPathFollowingResult::Success)
	{
		ownerBT->GetWorldTimerManager().SetTimer(TimerHandle, this,
			FTimerDelegate::TMethodPtr<UCalmBT>(&UCalmBT::ExeTreeInTimeIntervall),
			10.0f, false);
	}
}


