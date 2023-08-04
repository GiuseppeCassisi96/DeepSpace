// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UObject/NoExportTypes.h"
#include "BTInterface.h"
#include "TimerManager.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "CalmBT.generated.h"

/**
 * 
 */

/// <summary>
/// Is the class that represents the calm action. It is the default behavior.
/// The behavior defined by this BT is simple a map patrolling 
/// </summary>
UCLASS()
class DEEPSPACE_API UCalmBT : public UBTInterface 
{
	GENERATED_BODY()
public:
	UCalmBT() = default;
	/**
	 * @brief Check if the BT is stopped or not and then run the root task of tree. In this case
	 * I check also if the tree execution state is in the state "TryAgain", if this happens I run
	 * again the task (until fail implementation)
	 * @return The execution state of tree
	 */
	UFUNCTION()
	virtual ETaskExeState RunTree() override;
	/**
	 * @brief Initialize the tree
	 * @param owner The owner of the tree
	 * @param navSys The navigation system
	 */
	virtual void InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<AAlfredAIController> AIController) override;
	/**
	 * @brief Stop the tree
	 */
	virtual void StopTree() override;

	//TASK FUNCTIONS-----------------------------
	UFUNCTION()
	ETaskExeState IsReachable();
	UFUNCTION()
	ETaskExeState GoToRandPosition();
	//TASK FUNCTIONS-----------------------------

	//NODES OF TREE------------------------------
	TObjectPtr<USequenceBT> FirstSequence;
	UPROPERTY()
	TObjectPtr<UTaskBT> TaskIsReachable;
	UPROPERTY()
	TObjectPtr<UTaskBT> TaskGoToRandPosition;
	UPROPERTY()
	TObjectPtr<UUntilFailBT> CalmUntilFail;
	//NODES OF TREE------------------------------
	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstance;
	FNavLocation randLocation;
	FTimerHandle TimerHandle;
};
