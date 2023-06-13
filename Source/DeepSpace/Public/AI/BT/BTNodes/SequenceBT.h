// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BT/BTNodes/TaskBT.h"
#include "SequenceBT.generated.h"

/**
 * 
 */


/// <summary>
/// This class represents the sequence task. It execute a sequence of tasks if they don't fail   
/// </summary>
UCLASS()
class DEEPSPACE_API USequenceBT : public UTaskBT
{
	GENERATED_BODY()
public:
	USequenceBT() = default;
	/**
	 * @brief Runs the sequence of tasks
	 * @return The execution state of the sequence
	 */
	UFUNCTION()
	virtual ETaskExeState RunTask() override;
	/**
	 * @brief Is an array of tasks that will be executed if they don't fail
	 */
	UPROPERTY()
	TArray<UTaskBT*> Tasks;
	
	
};
