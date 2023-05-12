// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BT/TaskBT.h"
#include "SequenceBT.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API USequenceBT : public UTaskBT
{
	GENERATED_BODY()
public:
	USequenceBT() = default;
	UFUNCTION()
	virtual ETaskExeState RunTask() override;
	UPROPERTY()
	TArray<UTaskBT*> Tasks;
	
	
};
