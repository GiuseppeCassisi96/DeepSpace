// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FSMState.h"



int UFSMState::RunAction()
{
	if(Action.Execute() == -1)
	{
		throw Exception;
	}
	return 0;
}
