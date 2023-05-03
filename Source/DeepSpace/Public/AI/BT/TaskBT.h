// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TaskBT.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_RetVal(int, FTask);
UCLASS()
class DEEPSPACE_API UTaskBT : public UObject
{
	GENERATED_BODY()
public:
	UTaskBT() = default;
	virtual int RunTask();
	FTask Task;
};
