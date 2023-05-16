// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UObject/NoExportTypes.h"
#include "SequenceBT.h"
#include "BTInterface.h"
#include "TaskBT.h"
#include "UntilFailBT.h"
#include "TimerManager.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "CalmBT.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API UCalmBT : public UBTInterface 
{
	GENERATED_BODY()
public:
	UCalmBT() = default;
	UFUNCTION()
	virtual ETaskExeState RunTree() override;
	virtual void InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys) override;
	virtual void StopTree() override;
	UFUNCTION()
	ETaskExeState IsReachable();
	UFUNCTION()
	ETaskExeState GoToRandPosition();
	UFUNCTION()
	ETaskExeState ExeTreeInTimeIntervall();
	

	TObjectPtr<USequenceBT> firstSequence;
	UPROPERTY()
	TObjectPtr<UTaskBT> firstTask;
	UPROPERTY()
	TObjectPtr<UTaskBT> secondTask;
	UPROPERTY()
	TObjectPtr<UUntilFailBT> CalmUntilFail;
	FTimerManager TimerManager;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
};
