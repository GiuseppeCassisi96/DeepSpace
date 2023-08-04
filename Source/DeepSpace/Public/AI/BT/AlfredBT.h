// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackBT.h"
#include "CalmBT.h"
#include "WarningBT.h"
#include "NoticeSomethingBT.h"
#include "AlfredBT.generated.h"

/// <summary>
/// This class is an Alfred AI component responsible to initialize the behavior trees 
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEEPSPACE_API UAlfredBT : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAlfredBT();
	UPROPERTY()
	TObjectPtr<UCalmBT> CalmBT;
	UPROPERTY()
	TObjectPtr<UAttackBT> AttackBT;
	UPROPERTY()
	TObjectPtr<UWarningBT> WarningBT;
	UPROPERTY()
	TObjectPtr<UNoticeSomethingBT> NoticeSomethingBT;
	UPROPERTY()
	TArray<UBTInterface*> trees;
	void InitBehaviorTrees(TObjectPtr<ACharacter> owner, 
		TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<AAlfredAIController> AlfredAIController);


	

		
};
