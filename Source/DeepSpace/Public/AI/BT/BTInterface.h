// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BTNodes/TaskBT.h"
#include "BTNodes/SelectorBT.h"
#include "BTNodes/SequenceBT.h"
#include "BTNodes/UntilFailBT.h"
#include "GameFramework/Character.h"
#include "BTInterface.generated.h"
/// <summary>
/// Is the interface for the BT, every BT class inherits from this class. It
///	provides some general functions as RunTree, StopTree and InitTree that are
///	used from all child class
/// </summary>
UCLASS()
class DEEPSPACE_API UBTInterface : public UObject
{
public:
	GENERATED_BODY()
	UBTInterface();
	virtual ~UBTInterface() = default;
	/**
	 * @brief Run the BT
	 * @return The execution state of BT
	 */
	virtual ETaskExeState RunTree();
	/**
	 * @brief Stop the tree
	 */
	virtual void StopTree();
	/**
	 * @brief Initialize the tree
	 * @param owner The owner of the tree
	 * @param navSys The navigation system 
	 */
	virtual void InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys);
	/**
	 * @brief The execution state of my tree
	 */
	ETaskExeState TreeExeState;
	bool bIsStopped = false;
	UPROPERTY()
	TObjectPtr<UTaskBT> RootTask;
	UPROPERTY()
	TObjectPtr<ACharacter> ownerBT;
	UPROPERTY()
	TObjectPtr<UNavigationSystemV1> NavSys;
	UPROPERTY()
	TObjectPtr<AAIController> AIController;
	FNavLocation randLocation;
	FTimerHandle TimerHandle;

};
