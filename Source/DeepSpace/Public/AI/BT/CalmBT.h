// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UObject/NoExportTypes.h"
#include "SequenceBT.h"
#include "AlfredBT.h"
#include "TaskBT.h"
#include "UntilFailBT.h"
#include "TimerManager.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "CalmBT.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API UCalmBT : public UAlfredBT
{
	GENERATED_BODY()
public:
	UCalmBT();
	UFUNCTION()
	int IsReachable();
	UFUNCTION()
	int GoToRandPosition();
	UFUNCTION()
	virtual int RunTree() override;
	virtual void InitTree(ACharacter* owner, UNavigationSystemV1* navSys) override;
	UFUNCTION()
	int ExeTreeInTimeIntervall();
	UFUNCTION()
	void ResetTimer(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	virtual void StopTree() override;
	UPROPERTY()
	USequenceBT* firstSequence;
	UPROPERTY()
	UTaskBT* firstTask;
	UPROPERTY()
	UTaskBT* secondTask;
	UPROPERTY()
	UUntilFailBT* CalmUntilFail;
	FTimerManager TimerManager;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
};
