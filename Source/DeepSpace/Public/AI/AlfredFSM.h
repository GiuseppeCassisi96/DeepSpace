// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BT/BTInterface.h"
#include "AlfredFSM.generated.h"


class UAlfredBT;
/**
 * 
 */
UENUM(BlueprintType)
/// <summary>
/// Is the states of my FSM 
/// </summary>
enum class EEnemyState : uint8
{
	Calm = 0,//Is the default state
	Warning = 1,
	Attack = 2,
	NoticeSomething = 3
};

UCLASS()
class UAlfredFSMState : public UObject
{
	GENERATED_BODY()
public:
	TObjectPtr<UBTInterface> stateAction;
	TArray<EEnemyState> stateTransitions;
	FORCEINLINE ETaskExeState StayAction()
	{
		return stateAction->RunTree();
	}
	FORCEINLINE void ExitAction()
	{
		stateAction->StopTree();
	}

	FORCEINLINE void EnterAction()
	{
		//I reset the boolean var for sure
		stateAction->bIsStopped = false;
	}
};


/// <summary>
/// This class represent the FSM of my AI. It handles the state changing operation and runs the
///	current FSM state. It uses also a TMap data structure where the key is the enemy state and the value
///	is the FMSState. It has a Sensor Triggering Transaction Function (STTF) that performs the state transition
/// </summary>
UCLASS(ClassGroup = (FSM), meta = (BlueprintSpawnableComponent))
class DEEPSPACE_API UAlfredFSM : public UActorComponent
{
	GENERATED_BODY()
public:
	UAlfredFSM();
	void AddStates(TArray<UBTInterface*> BT);
	FORCEINLINE TMap<EEnemyState, UAlfredFSMState*>& GetFSM()
	{
		return FSM;
	}
	/**
	 * @brief Is the Sensor Triggering Transaction Function (STTF) that 'fires' the transition from
	 * one state to another, when the Alfred's Sensors are triggered. It checks also if the current
	 * state has as neighbour states the target state
	 * @param TargetState is the new state
	 */
	FORCEINLINE void GoToNewState(EEnemyState TargetState)
	{
		if(FSM[CurrentState]->stateTransitions.Contains(TargetState))
		{
			FSM[CurrentState]->ExitAction();
			CurrentState = TargetState;
			FSM[CurrentState]->EnterAction();
		}
			
	}
	FORCEINLINE EEnemyState GetCurrentState() { return CurrentState; }
	/**
	 * @brief Run the action of current state. If the action fails, it goes to the calm state
	 * (default state). So all states can go to default state. I do this to handle the task's
	 * failure
	 * @return returns the state of task
	 */
	FORCEINLINE ETaskExeState RunActionOfCurrentState()
	{
		ETaskExeState state = FSM[CurrentState]->StayAction();
		if(state == ETaskExeState::Fail)
		{
			GoToDefaultState();
		}
		return state;
	}
	/**
	 * @brief This function will be invoked in some cases, for example one of these cases is when
	 * the warning period finish
	 */
	UFUNCTION()
	FORCEINLINE void GoToDefaultState()
	{
		GoToNewState(EEnemyState::Calm);
		RunActionOfCurrentState();
	}
protected:
	EEnemyState CurrentState, InitialState;
	/**
	 * @brief It is basically the main structure of my FSM. I use a TMap as data structure,
	 * where the key is the enemy state and the value is the BT. I bind every enemy state whit
	 * a BT. UBTInterface is the interface of my all behavior trees, I use the polymorphism to
	 * call the correct version of BT which inherits from UBTInterface
	 */
	UPROPERTY(EditDefaultsOnly, Category = "States")
	TMap<EEnemyState, UAlfredFSMState*> FSM;
	


};
