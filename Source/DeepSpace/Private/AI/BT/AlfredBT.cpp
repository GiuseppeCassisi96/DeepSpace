// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/AlfredBT.h"

UAlfredBT::UAlfredBT()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAlfredBT::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UAlfredBT::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAlfredBT::InitBehaviorTrees(TArray<UBTInterface*> Trees, TObjectPtr<ACharacter> owner, 
	TObjectPtr<UNavigationSystemV1> navSys, TObjectPtr<AAlfredAIController> AlfredAIController)
{
	for(int i = 0; i < Trees.Num(); i++)
	{
		Trees[i]->InitTree(owner, navSys, AlfredAIController);
	}
}

