// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/TaskBT.h"

int UTaskBT::RunTask()
{
	return Task.Execute();
}

void UTaskBT::BindTask(UObject* InUserObject, const FName& InFunctionName)
{
	Task.BindUFunction(InUserObject, InFunctionName);
}
