// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTInterface.h"
#include "NavigationSystem.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Character.h"
#include "AttackBT.generated.h"

/**
 * 
 */

/// <summary>
/// Is the class that represents the attack action. It will be ran when the enemy see a hostile
/// entity. The behavior defined by this BT is enemy attacking 
/// </summary>
UCLASS()
class DEEPSPACE_API UAttackBT : public UBTInterface
{
	GENERATED_BODY()
public:
	UAttackBT() = default;
	/**
	 * @brief Check if the BT is stopped or not and then run the root task of tree
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
	ETaskExeState CheckLife();
	UFUNCTION()
	ETaskExeState CheckDistanceMoreThan();
	UFUNCTION()
	ETaskExeState GoTowardsThePlayer();
	UFUNCTION()
	ETaskExeState Attack();
	UFUNCTION()
	ETaskExeState WaitFunc();
	//TASK FUNCTIONS-----------------------------
	/**
	 * @brief It is UFUNCTION bound to a timer delegate that set the "bCanAttack" to true
	 */
	UFUNCTION()
	void SetCanAttack();

	//NODES OF TREE-----------------------------
	UPROPERTY()
	TObjectPtr<UTaskBT> checkLifeOfTarget;
	UPROPERTY()
	TObjectPtr<UTaskBT> checkDistanceMT;
	UPROPERTY()
	TObjectPtr<UTaskBT> followThePlayer;
	UPROPERTY()
	TObjectPtr<UTaskBT> attackThePlayer;
	UPROPERTY()
	TObjectPtr<UTaskBT> wait;
	TObjectPtr<USelectorBT> sequenceSelector;
	UPROPERTY()
	TObjectPtr<USequenceBT> firstSequence;
	UPROPERTY()
	TObjectPtr<USequenceBT> secondSequence;
	//NODES OF TREE-----------------------------

	//PARAMATERS OF TREE-----------------------------
	UPROPERTY()
	TObjectPtr<ACharacter> playerRefBT;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
	TSubclassOf<UDamageType> typeDamage;
	bool bCanAttack = true;
	FTimerHandle TimerHandle;

	//PARAMATERS OF TREE-----------------------------
};
