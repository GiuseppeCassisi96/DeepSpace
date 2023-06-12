// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BT/AttackBT.h"
#include "BT/CalmBT.h"
#include "BT/WarningBT.h"
#include "BT/NoticeSomethingBT.h"
#include "AlfredFSM.generated.h"


/**
 * 
 */
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Calm,
	Warning,
	Attack,
	Ally,
	NoticeSomething
};
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
	FORCEINLINE void GoToNewState(EEnemyState TargetState)
	{
		if (CurrentState != TargetState)
			States[CurrentState]->StopTree();
		CurrentState = TargetState;
	}
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
	


};
