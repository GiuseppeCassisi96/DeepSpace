// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BT/BTInterface.h"
#include "WarningBT.generated.h"

/**
 * 
 */


 /// <summary>
 /// Is the class that represents the warning action. It will be ran when the enemy has seen the player
 /// or reaches a noise source. The behavior defined by this BT is the hostile entity searching  
 /// </summary>
UCLASS()
class DEEPSPACE_API UWarningBT : public UBTInterface
{
public:
	GENERATED_BODY()
	/**
	 * @brief Check if the BT is stopped or not and then run the root task of tree In this case
	 * I check also if the tree execution state is in the state "TryAgain", if this happens I run
	 * again the task (until fail implementation)
	 * @return The execution state of tree
	 */
	UFUNCTION()
	virtual ETaskExeState RunTree() override;
	/**
	 * @brief Stop the tree
	 */
	virtual void StopTree() override;
	/**
	 * @brief Initialize the tree
	 * @param owner The owner of the tree
	 * @param navSys The navigation system
	 */
	virtual void InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<AAlfredAIController> AIController) override;
	//TASK FUNCTIONS-----------------------------
	UFUNCTION()
	ETaskExeState CanReachRandPos();
	UFUNCTION()
	ETaskExeState GoToRandPos();
	//TASK FUNCTIONS-----------------------------

	//NODES OF TREE------------------------------
	UPROPERTY()
	TObjectPtr<UTaskBT> TaskcanReachRandPos;
	UPROPERTY()
	TObjectPtr<UTaskBT> TaskGoToRandPos;
	UPROPERTY()
	TObjectPtr<USequenceBT> FirstSequence;
	UPROPERTY()
	TObjectPtr<UUntilFailBT> WarningUntilFail;
	//NODES OF TREE------------------------------
	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstance;
	/**
	 * @brief She is used to search the hostile entity near this location. This location
	 * can represent the noise source location or the last known position of hostile entity
	 */
	FVector Location;
	
};
