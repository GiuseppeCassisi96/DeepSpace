// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BT/BTNodes/TaskBT.h"
#include "DecoratorBT.generated.h"

/**
 * 
 */


 /// <summary>
 /// This class is the base interface for all decorators of my tree
 /// </summary>
UCLASS()
class DEEPSPACE_API UDecoratorBT : public UTaskBT
{
	GENERATED_BODY()
public:
	UDecoratorBT() = default;
	/**
	 * @brief Runs the task of decorator 
	 * @return The execution state of task 
	 */
	virtual ETaskExeState RunTask() override;
	
};
