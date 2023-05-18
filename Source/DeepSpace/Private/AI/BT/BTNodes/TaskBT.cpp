// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTNodes/TaskBT.h"

ETaskExeState UTaskBT::RunTask()
{
	if(Task.IsBound())
	{
		return Task.Execute();
	}
	return ETaskExeState::Fail;
}

