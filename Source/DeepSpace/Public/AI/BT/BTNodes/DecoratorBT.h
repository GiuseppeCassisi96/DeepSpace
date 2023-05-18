// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BT/BTNodes/TaskBT.h"
#include "DecoratorBT.generated.h"

/**
 * 
 */
UCLASS()
class DEEPSPACE_API UDecoratorBT : public UTaskBT
{
	GENERATED_BODY()
public:
	UDecoratorBT() = default;
	virtual ETaskExeState RunTask() override;
	
};
