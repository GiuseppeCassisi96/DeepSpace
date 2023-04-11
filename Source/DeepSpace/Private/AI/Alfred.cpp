// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Alfred.h"
#include "Enemy/MainEnemy.h"//Forward declaration 


// Sets default values for this component's properties
UAlfred::UAlfred()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UAlfred::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<AMainEnemy>(GetOwner());
	if(owner)
	{
		ControllerNPC = Cast<AAIController>(owner->GetController());
		ControllerNPC->ReceiveMoveCompleted.AddDynamic(this, &UAlfred::NPCReachsTheLocation);
		owner->ViewBox->OnComponentBeginOverlap.AddDynamic(this, &UAlfred::StartVisualSensors);
		owner->ViewBox->OnComponentEndOverlap.AddDynamic(this, &UAlfred::StopVisualSensors);
	}
}

// Called every frame
void UAlfred::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	EnemyView();
}

void UAlfred::NPCReachsTheLocation(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (bHasSeen && Result == EPathFollowingResult::Success)
	{
		//Pass to warning state: He has seen the player, but now doesn't see him anymore
		bHasSeen = false;
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, TEXT("Warning after seen"));
	}
}


void UAlfred::StartVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
                                 int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults)
{
	if (otherActor)
	{
		PlayerCharacter = Cast<AMainCharacter>(otherActor);
		if (PlayerCharacter)
		{
			bIsPlayerInTheViewBox = true;
		}
	}
}

void UAlfred::EnemyView()
{
	 
	if(bIsPlayerInTheViewBox)
	{
		bonesOfPlayer = PlayerCharacter->GetMainCharacterBones();
		const FVector npcEyesLocation = owner->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f);
		for(auto boneLocation: bonesOfPlayer)
		{
			
			UKismetSystemLibrary::LineTraceSingle(this, npcEyesLocation,
				boneLocation, UEngineTypes::ConvertToTraceType(ECC_Camera), true,
				actorToIgnore, EDrawDebugTrace::ForDuration, HitResult, true
				, FLinearColor::Red, FLinearColor::Black, 3.0f);
			if (Cast<AMainCharacter>(HitResult.GetActor()))
			{
				seeCount++;
			}
		}
	}
	//I map the seeCount value into a membership value of my fuzzy set between 0 and 1
	SeeSet.Fuzzification(6, seeCount);

	if (SeeSet.Defuzzification(SeeSet.Set, 0.65f))
	{
		//Pass to attack state: The enemy sees you !!
		//@todo Move npc movement in Behavior Tree. 
		const FVector newLocationToReach = PlayerCharacter->GetActorLocation();
		ControllerNPC->MoveToLocation(newLocationToReach, 100.0f);
		bHasSeen = true;
	}
	else if (SeeSet.Defuzzification(SeeSet.Set, 0.39f))
	{
		//Pass to warning state: The enemy has seen something and getting worried
		if(!bHasSeen)
		{
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Warning"));
		}
	}
	seeCount = 0;
}

void UAlfred::StopVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int otherBodyIndex)
{
	if (otherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(otherActor);
		if (MainCharacter)
		{
			bIsPlayerInTheViewBox = false;
			seeCount = 0;
		}
	}
}

