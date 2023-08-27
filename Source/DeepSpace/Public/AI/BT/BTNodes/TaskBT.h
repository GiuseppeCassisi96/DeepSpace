// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TaskBT.generated.h"

/// <summary>
/// Is an enum that represent the execution state of a task 
/// </summary>
UENUM()
enum class ETaskExeState
{
	Success = 0,
	Fail = -1,
	TryAgain = 1,
	Stopped = 2
};
/// <summary>
/// It is the type of delegate. This type in particular defines the type of the return value (ETaskExeState).
/// So every function that is bound to a delegate with this type must have an ETaskExeState return value and
/// zero params
/// </summary>
DECLARE_DELEGATE_RetVal(ETaskExeState, FTask);

/// <summary>
/// This class is the base interface for all nodes of my tree. Represents a basic task of my tree (Action
/// or Condition)
/// </summary>
UCLASS()
class DEEPSPACE_API UTaskBT : public UObject
{
	GENERATED_BODY()
public:
	UTaskBT() = default;
	/**
	 * @brief Runs the task
	 * @return The execution state of task 
	 */
	virtual ETaskExeState RunTask();
	/**
	 * @brief It is the delegate that will be bound to a function 
	 */
	FTask Task;
	
};
