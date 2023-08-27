// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTNodes/SequenceBT.h"

#include "Engine/Engine.h"

ETaskExeState USequenceBT::RunTask()
{
	for (int i = 0; i < Tasks.Num(); i++)
	{
		//Calls the "RunTask" of the UTaskBT class
		//If the task fails all the sequence fails
		if(Tasks[i]->RunTask() == ETaskExeState::Fail)
		{
			return ETaskExeState::Fail;
		}
	}
	return ETaskExeState::Success;
}
