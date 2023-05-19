// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BT/BTInterface.h"
#include "NoticeSomethingBT.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API UNoticeSomethingBT : public UBTInterface
{
	GENERATED_BODY()
public:
	virtual void InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys, FVector location);
	virtual ETaskExeState RunTree() override;
	virtual void StopTree() override;
	UFUNCTION()
	ETaskExeState CanReach();
	UFUNCTION()
	ETaskExeState GoToLocation();

	UPROPERTY()
	TObjectPtr<UTaskBT> TaskCanReach;
	UPROPERTY()
	TObjectPtr<UTaskBT> TaskGoToLocation;
	UPROPERTY()
	TObjectPtr<USequenceBT> FirstSequence;
	FVector SourceLocation;
	
};
