// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FSMState.h"



bool UFSMState::RunAction()
{
	if(Action.Execute())
	{
		return true;
	}
	throw Exception;
}
