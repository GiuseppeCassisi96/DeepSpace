// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TaskBT.generated.h"

UENUM()
enum class ETaskExeState
{
	Success,
	Fail,
	TryAgain
};
DECLARE_DELEGATE_RetVal(ETaskExeState, FTask);
UCLASS()
class DEEPSPACE_API UTaskBT : public UObject
{
	GENERATED_BODY()
public:
	UTaskBT() = default;
	virtual ETaskExeState RunTask();

	FTask Task;
	
};
