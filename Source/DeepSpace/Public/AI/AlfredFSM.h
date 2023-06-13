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
/// <summary>
/// Is the states of my FSM 
/// </summary>
enum class EEnemyState : uint8
{
	Calm,//Is the default state
	Warning,
	Attack,
	NoticeSomething
};
/// <summary>
/// This class represent the FSM of my AI. It handles the state changing operation and runs the
///	current FSM. It uses also a TMap data structure where the key is the enemy state and the value
///	is the BT. It has a Sensor Triggering Transaction Function (STTF) that performs the state transition
/// </summary>
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
	 * @brief Run the action of current state. If the action fails, it goes to the calm state
	 * @return returns the state of task
	 */
	ETaskExeState RunActionOfCurrentState();
protected:
	EEnemyState CurrentState, InitialState;
	/**
	 * @brief It is basically the main structure of my FSM. I use a TMap as data structure,
	 * where the key is the enemy state and the value is the BT. I bind every enemy state whit
	 * a BT. UBTInterface is the interface of my all behavior trees, I use the polymorphism to
	 * call the correct version of BT which inherits from UBTInterface
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess = true), Category = "States")
	TMap<EEnemyState, UBTInterface*> States;
	


};
