// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/UntilFailBT.h"



int UUntilFailBT::RunTask()
{
	if(childTask->RunTask() == -1)
	{
		return 0;
	}
	return 1;
}

