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
UCLASS()
class DEEPSPACE_API UAttackBT : public UBTInterface
{
	GENERATED_BODY()
public:
	UAttackBT() = default;
	UFUNCTION()
	virtual ETaskExeState RunTree() override;
	virtual void StopTree() override;
	virtual void InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<ACharacter> playerRef, TSubclassOf<UDamageType> type);
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
	UFUNCTION()
	void SetCanAttack();

	//NODES OF TREE
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


	//PARAMATERS OF TREE
	UPROPERTY()
	TObjectPtr<ACharacter> playerRefBT;
	
	TSubclassOf<UDamageType> typeDamage;
	bool bCanAttack = true;

	//ATTACK TIMER
	FTimerHandle TimerHandle;
	
};
