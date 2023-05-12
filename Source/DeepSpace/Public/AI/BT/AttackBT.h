// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTInterface.h"
#include "SelectorBT.h"
#include "SequenceBT.h"
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
	UAttackBT();
	UFUNCTION()
	virtual ETaskExeState RunTree() override;
	virtual void StopTree() override;
	virtual void InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<ACharacter> playerRef, TSubclassOf<UDamageType> type);
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

	TObjectPtr<UTaskBT> checkDistanceMT;
	TObjectPtr<UTaskBT> followThePlayer;
	TObjectPtr<UTaskBT> attackThePlayer;
	TObjectPtr<UTaskBT> wait;

	TObjectPtr<USelectorBT> sequenceSelector;
	TObjectPtr<USequenceBT> firstSequence;
	TObjectPtr<USequenceBT> secondSequence;
	TObjectPtr<ACharacter> playerRefBT;
	
	TSubclassOf<UDamageType> typeDamage;
	bool bCanAttack = true;
	FTimerHandle TimerHandle;
	
};
