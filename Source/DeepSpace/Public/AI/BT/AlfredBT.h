// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "TaskBT.h"
#include "GameFramework/Character.h"
#include "UObject/NoExportTypes.h"
#include "AlfredBT.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DEEPSPACE_API UAlfredBT : public UObject
{
	GENERATED_BODY()
public:
	virtual int RunTree();
	virtual void StopTree();
	virtual void InitTree(ACharacter* owner, UNavigationSystemV1* navSys);
	int TreeExeCode;
	bool bIsStopped = false;
	UPROPERTY()
	UTaskBT* RootTask;
	UPROPERTY()
	ACharacter* ownerBT;
	UPROPERTY()
	UNavigationSystemV1* NavSys;
	UPROPERTY()
	AAIController* AIController;
	FNavLocation randLocation;
};
