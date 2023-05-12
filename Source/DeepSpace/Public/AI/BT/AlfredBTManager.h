// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBT.h"
#include "CalmBT.h"
#include "AI/FSMState.h"
#include "Components/ActorComponent.h"
#include "AlfredBTManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEEPSPACE_API UAlfredBTManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAlfredBTManager();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
