// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "AI/BT/DecoratorBT.h"
#include "UntilFailBT.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API UUntilFailBT : public UDecoratorBT
{
	GENERATED_BODY()
public:
	UUntilFailBT() = default;
	virtual int RunTask() override;
	UPROPERTY()
	UTaskBT* childTask;
	
};
