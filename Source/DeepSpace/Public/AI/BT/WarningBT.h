// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BT/BTInterface.h"
#include "WarningBT.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API UWarningBT : public UBTInterface
{
public:
	GENERATED_BODY()
	UFUNCTION()
	virtual ETaskExeState RunTree() override;
	virtual void StopTree() override;
	virtual void InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys) override;

	UFUNCTION()
	ETaskExeState CanReachRandPos();
	UFUNCTION()
	ETaskExeState GoToRandPos();
	UPROPERTY()
	TObjectPtr<UTaskBT> TaskcanReachRandPos;
	UPROPERTY()
	TObjectPtr<UTaskBT> TaskGoToRandPos;
	UPROPERTY()
	TObjectPtr<USequenceBT> FirstSequence;
	UPROPERTY()
	TObjectPtr<UUntilFailBT> CalmUntilFail;
	FVector Location;

	FVector dest;
	
};
