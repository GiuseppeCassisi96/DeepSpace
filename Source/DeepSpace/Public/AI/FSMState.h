// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFailException.h"
#include "UObject/NoExportTypes.h"
#include "FSMState.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Calm,
	Hearing,
	Warning,
	Attack,
	Ally
};

enum class ETaskExeState;
DECLARE_DELEGATE_RetVal(ETaskExeState, FActionState);
/**
 * 
 */
UCLASS()
class DEEPSPACE_API UFSMState : public UObject
{
	GENERATED_BODY()
public:
	UFSMState() = default;
	ETaskExeState RunAction();
	FActionState Action;
protected:
	ActionFailException Exception;
};
