// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMState.h"
#include "AI/BT/AlfredBTManager.h"
#include "AlfredFSM.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = (FSM), meta = (BlueprintSpawnableComponent))
class DEEPSPACE_API UAlfredFSM : public UActorComponent
{
	GENERATED_BODY()
public:
	UAlfredFSM();
	TMap<EEnemyState, UBTInterface*>& GetStates();
	/**
	 * @brief Is a Sensor Triggering Transaction Function (STTF) that 'fires' the transition from
	 * one state to another, when the Alfred Sensors are triggered
	 * @param TargetState is the new state
	 */
	FORCEINLINE void GoToNewState(EEnemyState TargetState) { CurrentState = TargetState; }
	FORCEINLINE EEnemyState GetCurrentState() { return CurrentState; }
	/**
	 * @brief Run the action of current state. If the action fails, it throws an 'ActionFailException'
	 * @return returns ETaskExeState::Fail if the action fails, ETaskExeState::Success otherwise
	 */
	ETaskExeState RunActionOfCurrentState();
	void StopAction();
protected:
	EEnemyState CurrentState, InitialState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess = true), Category = "States")
	TMap<EEnemyState, UBTInterface*> States;
	UPROPERTY()
	TObjectPtr<UFSMState> CalmState;
	UPROPERTY()
	TObjectPtr<UFSMState> HearingState;
	UPROPERTY()
	TObjectPtr<UFSMState> WarningState;
	UPROPERTY()
	TObjectPtr<UFSMState> AttackState;
	UPROPERTY()
	TObjectPtr<UFSMState> AllyState;
	


};
