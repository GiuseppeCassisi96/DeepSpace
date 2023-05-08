// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/SelectorBT.h"

int USelectorBT::RunTask()
{
	for(int i = 0; i < Tasks.Num(); i++)
	{
		if(Tasks[i]->RunTask() != -1)
		{
			return 0;
		}
	}
	return -1;
}
