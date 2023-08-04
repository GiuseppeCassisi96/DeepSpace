// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Alfred.h"
#include "AI/AlfredSensing.h" //
#include "Enemy/MainEnemy.h"//Forward declaration




// Sets default values for this component's properties
UAlfred::UAlfred()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//AI components creation
	AlfredBT = CreateDefaultSubobject<UAlfredBT>(TEXT("AlfredBT"));
	AlfredFSM = CreateDefaultSubobject<UAlfredFSM>(TEXT("AlfredFSM"));
	AlfredSensing = CreateDefaultSubobject<UAlfredSensing>(TEXT("AlfredSensing"));
}



//INIT AI FUNCTION: This function is responsible to initialize the AI.
void UAlfred::InitAI()
{
	owner = Cast<AMainEnemy>(GetOwner());
	if(!owner)
		return;
	AlfredSensing->SetOwner(owner);

	//I bind the sensory functions with the box and sphere delegates  
	owner->ViewBox->OnComponentBeginOverlap.AddDynamic(AlfredSensing.Get(), &UAlfredSensing::StartVisualSensors);
	owner->ViewBox->OnComponentEndOverlap.AddDynamic(AlfredSensing.Get(), &UAlfredSensing::StopVisualSensors);
	owner->HearingSphere->OnComponentBeginOverlap.AddDynamic(AlfredSensing.Get(), &UAlfredSensing::StartHearSensors);
	owner->HearingSphere->OnComponentEndOverlap.AddDynamic(AlfredSensing.Get(), &UAlfredSensing::StopHearSensors);

	owner->ControllerNPC->ReceiveMoveCompleted.AddDynamic(owner->ControllerNPC, &AAlfredAIController::NPCReachesTheLocation);

	//Initialize and add to the FSM the Behavior trees
	AlfredBT->InitBehaviorTrees(owner,owner->ControllerNPC->GetNavSys(), owner->ControllerNPC);
	AlfredFSM->AddStates(AlfredBT->trees);

	//Run the default state (Calm state)
	AlfredFSM->RunActionOfCurrentState();
}
//INIT AI FUNCTION:-----------------------------------------------------------------------------------------


// Called when the game starts
void UAlfred::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UAlfred::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(owner)
	{
		AlfredSensing->EnemyView(); //RUN THE VIEW SENSOR
		AlfredSensing->EnemyHearing(); //RUN THE HEAR SENSOR
	}
		
}

void UAlfred::GoToCalmStateAfterDestroy(AMainEnemy* Enemy)
{
	//Remove the enemy destroyed 
	int n1 = AlfredSensing->EnemyData.CharactersHeard.Remove(Enemy);
	int n2 = AlfredSensing->EnemyData.CharactersSeen.Remove(Enemy);
	if(n1 + n2 > 0 && AlfredFSM->GetCurrentState() == EEnemyState::Attack)
	{
		AlfredSensing->SeeSet.Set.membershipValue = 0.0f;
		AlfredFSM->GoToDefaultState();
	}
	
}








