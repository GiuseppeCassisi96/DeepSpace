// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMState.h"
#include "UObject/NoExportTypes.h"
#include "AlfredFSM.generated.h"


/**
 * 
 */
UCLASS()
class DEEPSPACE_API UAlfredFSM : public UObject
{
	GENERATED_BODY()
public:
	UAlfredFSM();
	TMap<EEnemyState, UFSMState*>& GetStates();
	/**
	 * @brief Is a Sensor Triggering Transaction Function (STTF) that 'fires' the transition from
	 * one state to another, when the Alfred Sensors are triggered
	 * @param TargetState is the new state
	 */
	FORCEINLINE void GoToNewState(EEnemyState TargetState) { CurrentState = TargetState; };
	/**
	 * @brief Run the action of current state. If the action fails, it throws an 'ActionFailException'
	 * @return returns false if the action fails, true otherwise
	 */
	bool RunActionOfCurrentState();
	EEnemyState CurrentState, InitialState;
protected:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	TMap<EEnemyState, UFSMState*> States;
	UPROPERTY()
	UFSMState* CalmState;
	UPROPERTY()
	UFSMState* HearingState;
	UPROPERTY()
	UFSMState* WarningState;
	UPROPERTY()
	UFSMState* AttackState;
	UPROPERTY()
	UFSMState* AllyState;


};
