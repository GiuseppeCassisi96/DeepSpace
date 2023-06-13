// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTNodes/SelectorBT.h"

ETaskExeState USelectorBT::RunTask()
{
	for(int i = 0; i < Tasks.Num(); i++)
	{
		//If the task successes a Success status will be returned 
		if(Tasks[i]->RunTask() != ETaskExeState::Fail)
		{
			return ETaskExeState::Success;
		}
	}
	return ETaskExeState::Fail;
}
