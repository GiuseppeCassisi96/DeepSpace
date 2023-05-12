// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "TaskBT.h"
#include "GameFramework/Character.h"
#include "BTInterface.generated.h"
UCLASS()
class DEEPSPACE_API UBTInterface : public UObject
{
public:
	GENERATED_BODY()
	virtual ~UBTInterface() = default;
	UBTInterface();
	virtual ETaskExeState RunTree();
	virtual void StopTree();
	virtual void InitTree(TObjectPtr<ACharacter> owner, TObjectPtr<UNavigationSystemV1> navSys);
	ETaskExeState TreeExeState;
	bool bIsStopped = false;
	UPROPERTY()
	TObjectPtr<UTaskBT> RootTask;
	UPROPERTY()
	TObjectPtr<ACharacter> ownerBT;
	UPROPERTY()
	TObjectPtr<UNavigationSystemV1> NavSys;
	UPROPERTY()
	TObjectPtr<AAIController> AIController;
	FNavLocation randLocation;

};
