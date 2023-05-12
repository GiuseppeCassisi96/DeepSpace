// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/SequenceBT.h"

#include "Engine/Engine.h"

ETaskExeState USequenceBT::RunTask()
{
	for (int i = 0; i < Tasks.Num(); i++)
	{
		if(Tasks[i]->RunTask() == ETaskExeState::Fail)
		{
			return ETaskExeState::Fail;
		}
	}
	return ETaskExeState::Success;
}
