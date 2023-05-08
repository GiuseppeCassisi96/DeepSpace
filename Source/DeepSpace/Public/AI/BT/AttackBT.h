// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlfredBT.h"
#include "SelectorBT.h"
#include "SequenceBT.h"
#include "NavigationSystem.h"
#include "UObject/NoExportTypes.h"
#include "SequenceBT.h"
#include "GameFramework/Character.h"
#include "AttackBT.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API UAttackBT : public UAlfredBT
{
	GENERATED_BODY()
public:
	UAttackBT();
	UFUNCTION()
	virtual int RunTree() override;
	virtual void StopTree() override;
	virtual void InitTree(ACharacter* owner, UNavigationSystemV1* navSys, ACharacter* playerRef, TSubclassOf<UDamageType> type);
	UFUNCTION()
	int CheckDistanceMoreThan();
	UFUNCTION()
	int GoTowardsThePlayer();
	UFUNCTION()
	int Attack();
	UFUNCTION()
	int WaitFunc();
	UFUNCTION()
	void SetCanAttack();

	UPROPERTY()
	UTaskBT* checkDistanceMT;
	UPROPERTY()
	UTaskBT* followThePlayer;
	UPROPERTY()
	UTaskBT* attackThePlayer;
	UPROPERTY()
	UTaskBT* wait;

	UPROPERTY()
	USelectorBT* sequenceSelector;
	UPROPERTY()
	USequenceBT* firstSequence;
	UPROPERTY()
	USequenceBT* secondSequence;
	UPROPERTY()
	ACharacter* playerRefBT;
	
	TSubclassOf<UDamageType> typeDamage;
	bool bCanAttack = true;
	FTimerHandle TimerHandle;
	
};
