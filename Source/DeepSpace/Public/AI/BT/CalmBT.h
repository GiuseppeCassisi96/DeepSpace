// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Character.h"
#include "SequenceBT.h"
#include "TaskBT.h"
#include "UntilFailBT.h"
#include "TimerManager.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "CalmBT.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API UCalmBT : public UObject
{
	GENERATED_BODY()
public:
	UCalmBT();
	UFUNCTION()
	int IsReachable();
	UFUNCTION()
	int GoToRandPosition();
	UFUNCTION()
	int RunTree();
	void InitTree(ACharacter* owner, UNavigationSystemV1* navSys);
	UFUNCTION()
	int ExeTreeInTimeIntervall();
	UFUNCTION()
	void ResetTimer(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	UPROPERTY()
	USequenceBT* firstSequence;
	UPROPERTY()
	UTaskBT* firstTask;
	UPROPERTY()
	UTaskBT* secondTask;
	UPROPERTY()
	UUntilFailBT* CalmUntilFail;
	UPROPERTY()
	UTaskBT* RootTask;
	UPROPERTY()
	UNavigationSystemV1* NavSys;
	UPROPERTY()
	ACharacter* ownerBT;
	UPROPERTY()
	AAIController* AIController;
	FTimerManager TimerManager;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	FNavLocation randLocation;
	int TreeExeCode;

};
