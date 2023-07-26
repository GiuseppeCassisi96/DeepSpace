// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BTInterface.h"
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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitBehaviorTrees(TArray<UBTInterface*> Trees, TObjectPtr<ACharacter> owner, 
		TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<AAlfredAIController> AlfredAIController);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	

		
};
