// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BT/BTInterface.h"
#include "NoticeSomethingBT.generated.h"

/**
 * 
 */

 /// <summary>
 /// Is the class that represents the noticeSomething action. It will be ran when the enemy perceives a noise.
 /// The behavior defined by this BT is the simple movement towards the noise source.  
 /// </summary>
UCLASS()
class DEEPSPACE_API UNoticeSomethingBT : public UBTInterface
{
	GENERATED_BODY()
public:
	/**
	 * @brief Initialize the tree
	 * @param owner The owner of the tree
	 * @param navSys The navigation system
	 * @param location The noise source location
	 */
	virtual void InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys, FVector location);
	/**
	 * @brief Check if the BT is stopped or not and then run the root task of tree
	 * @return The execution state of tree
	 */
	virtual ETaskExeState RunTree() override;
	/**
	 * @brief Stop the tree
	 */
	virtual void StopTree() override;

	//TASK FUNCTIONS-----------------------------
	UFUNCTION()
	ETaskExeState CanReach();
	UFUNCTION()
	ETaskExeState GoToLocation();
	//TASK FUNCTIONS-----------------------------

	//NODES OF TREE------------------------------
	UPROPERTY()
	TObjectPtr<UTaskBT> TaskCanReach;
	UPROPERTY()
	TObjectPtr<UTaskBT> TaskGoToLocation;
	UPROPERTY()
	TObjectPtr<USequenceBT> FirstSequence;
	//NODES OF TREE------------------------------

	//PARAMATERS OF TREE-----------------------------
	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstance;
	FVector SourceLocation;
	//PARAMATERS OF TREE-----------------------------
};
