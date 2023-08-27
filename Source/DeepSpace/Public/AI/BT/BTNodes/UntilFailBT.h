// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "AI/BT/BTNodes/DecoratorBT.h"
#include "UntilFailBT.generated.h"

/**
 * 
 */

/// <summary>
/// This class represent the until fail decorator 
/// </summary>
UCLASS()
class DEEPSPACE_API UUntilFailBT : public UDecoratorBT
{
	GENERATED_BODY()
public:
	UUntilFailBT();
	/**
	 * @brief Runs the child task, if the task successes returns a "TryAgain" exe status
	 * @return The exe status of the until fail decorator 
	 */
	virtual ETaskExeState RunTask() override;
	UPROPERTY()
	TObjectPtr<UTaskBT> childTask;
	
};
