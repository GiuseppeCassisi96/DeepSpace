// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BT/BTNodes/TaskBT.h"
#include "SelectorBT.generated.h"

/**
 * 
 */


 /// <summary>
 /// This class represents the selector task. It execute a task, if this fails the selector executes the next
 /// one. If all tasks fail the selector fails 
 /// </summary>
UCLASS()
class DEEPSPACE_API USelectorBT : public UTaskBT
{
	GENERATED_BODY()
public:
	USelectorBT() = default;
	/**
	 * @brief Runs the first task. If the first task fails the the selector runs the next one and so on
	 * @return The execution state of selector
	 */
	UFUNCTION()
	virtual ETaskExeState RunTask() override;
	/**
	 * @brief Is an array of tasks
	 */
	UPROPERTY()
	TArray<UTaskBT*> Tasks;
	
};
