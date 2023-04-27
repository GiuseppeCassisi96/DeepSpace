// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionFailException.h"
#include "UObject/NoExportTypes.h"
#include "FSMState.generated.h"
DECLARE_DELEGATE_RetVal(bool, FActionState);
UENUM()
enum class EEnemyState
{
	Calm = 0,
	Hearing = 1,
	Warning = 2,
	Attack = 3,
	Ally = 4
};
/**
 * 
 */
UCLASS()
class DEEPSPACE_API UFSMState : public UObject
{
	GENERATED_BODY()
public:
	UFSMState() = default;
	bool RunAction();
	FActionState Action;
protected:
	ActionFailException Exception;
};
