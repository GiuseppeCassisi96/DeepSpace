// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/UntilFailBT.h"


UUntilFailBT::UUntilFailBT()
{
	childTask = NewObject<UTaskBT>();
}

ETaskExeState UUntilFailBT::RunTask()
{
	if(childTask->RunTask() == ETaskExeState::Fail)
	{
		return ETaskExeState::Success;
	}
	return ETaskExeState::TryAgain;
}

