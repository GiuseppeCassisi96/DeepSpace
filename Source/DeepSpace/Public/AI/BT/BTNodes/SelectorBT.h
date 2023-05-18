// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BT/BTNodes/TaskBT.h"
#include "SelectorBT.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API USelectorBT : public UTaskBT
{
	GENERATED_BODY()
public:
	USelectorBT() = default;
	UFUNCTION()
	virtual ETaskExeState RunTask() override;
	UPROPERTY()
	TArray<UTaskBT*> Tasks;
	
};
