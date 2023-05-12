// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FSMState.h"
#include "AI/BT/AlfredBTManager.h"


ETaskExeState UFSMState::RunAction()
{
	if(Action.IsBound())
	{
		if (Action.Execute() == ETaskExeState::Fail)
		{
			throw Exception;
		}
		return ETaskExeState::Success;
	}
	return ETaskExeState::Fail;
	
}
