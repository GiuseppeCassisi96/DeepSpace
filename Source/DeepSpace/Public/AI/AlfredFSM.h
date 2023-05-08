// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMState.h"
#include "BT/AlfredBTManager.h"
#include <iostream>
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "UObject/NoExportTypes.h"
#include "AlfredFSM.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DEEPSPACE_API UAlfredFSM : public UActorComponent
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
	FORCEINLINE void GoToNewState(EEnemyState TargetState) { CurrentState = TargetState; }
	/**
	 * @brief Run the action of current state. If the action fails, it throws an 'ActionFailException'
	 * @return returns -1 if the action fails, 0 otherwise
	 */
	int RunActionOfCurrentState();
	void BindStates(UAlfredBTManager* BTManager);
	EEnemyState CurrentState, InitialState;
	UPROPERTY()
	ACharacter* ownerFSM;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess = true), Category = "States")
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
