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
		owner->HearingSphere->OnComponentBeginOverlap.AddDynamic(this, &UAlfred::StartHearSensors);
		owner->HearingSphere->OnComponentEndOverlap.AddDynamic(this, &UAlfred::StopHearSensors);
		navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		AlfredFSM = NewObject<UAlfredFSM>();
	}
}

// Called every frame
void UAlfred::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	EnemyView();
	EnemyHearing();
}

void UAlfred::NPCReachsTheLocation(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (bHasSeen && Result == EPathFollowingResult::Success && !SeeSet.Defuzzification(0.65f))
	{
		//Pass to warning state: He has seen the player, but now doesn't see him anymore
		GEngine->AddOnScreenDebugMessage(-1, 2.0f,
			FColor::Red, TEXT("WARNING AFTER SEEN"));
		AlfredFSM->GoToNewState(EEnemyState::Warning);
		//AlfredFSM->RunActionOfCurrentState();
		bHasSeen = false;
	}
}

void UAlfred::ItemHitNearEnemy(FVector hitLocation, float itemNoisiness)
{
	/*The 'pathCost' indicates the noise that the enemy perceives. This noise use a pathfinding system in order
	 *to take into account the environment of the level. Less is the 'pathCost' value more is the noise perceived. 
	 */
	float pathCost;
	if(!bHasSeen)
	{
		navSys->GetPathCost(this, owner->GetActorLocation(),
			hitLocation, pathCost);
		//I'm sure that the pathCost is between 0.0f and 1500.0f
		pathCost = std::clamp(pathCost - itemNoisiness, 0.0f, 1500.0f);
		NonHearSet.Fuzzification(1500.0f, pathCost);
		/*I obtain the HearSet simply denying the NonHearSet. When the membership value of NonHearSet is high
		 *the membership value of HearSet is low and vice versa. I overload the operator '!'*/
		NonHearSet.Set.membershipValue = !NonHearSet;
		/*Now we have the negated value of membership value of NonHearSet, so we have the membership value
		 * of HearSet*/
		DrawDebugPoint(GetWorld(), hitLocation, 20.0f, FColor::Red, false, 20.0f);
		if (NonHearSet.Defuzzification(0.30))
		{
			//Pass to hear state: The enemy hears something !!
			AlfredFSM->GoToNewState(EEnemyState::Hearing);
			//AlfredFSM->RunActionOfCurrentState();
			//@todo Move npc movement in Behavior Tree. 
			const FVector newLocationToReach = hitLocation;
			ControllerNPC->MoveToLocation(newLocationToReach, 100.0f);
		}
	}
	
}


void UAlfred::StartVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
                                 int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults)
{
	if (otherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(otherActor);
		if (MainCharacter)
		{
			EnemyData.PlayerCharacter = MainCharacter;
			bIsPlayerInTheViewBox = true;
		}
	}
}

void UAlfred::EnemyView()
{
	 
	if(bIsPlayerInTheViewBox)
	{
		EnemyData.bonesOfPlayer = EnemyData.PlayerCharacter->GetMainCharacterBones();
		const FVector npcEyesLocation = owner->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f);
		for(auto boneLocation: EnemyData.bonesOfPlayer)
		{
			UKismetSystemLibrary::LineTraceSingle(this, npcEyesLocation,
				boneLocation, UEngineTypes::ConvertToTraceType(ECC_Camera), true,
				actorToIgnore, EDrawDebugTrace::None, HitResult, true);
			if (Cast<AMainCharacter>(HitResult.GetActor()))
			{
				seeCount++;
			}
		}
	}
	//I map the seeCount value into a membership value of my fuzzy set between 0 and 1
	SeeSet.Fuzzification(6, seeCount);

	if (SeeSet.Defuzzification( 0.65f))
	{
		//Pass to attack state: The enemy sees you !!
		AlfredFSM->GoToNewState(EEnemyState::Attack);
		//AlfredFSM->RunActionOfCurrentState();
		//@todo Move npc movement in Behavior Tree. 
		const FVector newLocationToReach = EnemyData.PlayerCharacter->GetActorLocation();
		ControllerNPC->MoveToLocation(newLocationToReach, 100.0f);
		bHasSeen = true;
	}
	else if (SeeSet.Defuzzification( 0.39f))
	{
		//Pass to warning state: The enemy has seen something and getting worried
		AlfredFSM->GoToNewState(EEnemyState::Warning);
		//AlfredFSM->RunActionOfCurrentState();
		if(!bHasSeen)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Warning"));
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

void UAlfred::StartHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults)
{
	if (otherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(otherActor);
		AThrowableItem* ThrowableItem = Cast<AThrowableItem>(otherActor);
		//When the player enters in the hear sphere
		if (MainCharacter)
		{
			EnemyData.PlayerCharacter = MainCharacter;
			bIsPlayerInTheHearingSphere = true;
		}
		if(ThrowableItem)
		{

			ThrowableItem->OnThrowableItemEvent.AddDynamic(this, &UAlfred::ItemHitNearEnemy);
		}
		
	}
}

void UAlfred::EnemyHearing()
{
	if(bIsPlayerInTheHearingSphere)
	{
		/*The 'pathCost' indicates the noise that the enemy perceives. This noise use a pathfinding system in order
	    *to take into account the environment of the level. Less is the 'pathCost' value more is the noise perceived.
	    */
		float pathCost;
		
		if(EnemyData.PlayerCharacter->state != PlayerAnimState::Crouch && !bHasSeen)
		{
			navSys->GetPathCost(this, owner->GetActorLocation(),
				EnemyData.PlayerCharacter->GetActorLocation(), pathCost);
			float reduceFactor = static_cast<float>(EnemyData.PlayerCharacter->state);
			//I'm sure that the pathCost is between 0.0f and 1500.0f
			pathCost = std::clamp(pathCost - reduceFactor, 0.0f, 1500.0f);
			NonHearSet.Fuzzification(1500.0f, pathCost);
			/*I obtain the HearSet simply denying the NonHearSet. When the membership value of NonHearSet is high
			 *the membership value of HearSet is low and vice versa. I overload the operator '!'*/
			NonHearSet.Set.membershipValue = !NonHearSet;
			/*Now we have the negated value of membership value of NonHearSet, so we have the membership value
			 * of HearSet*/
			if (NonHearSet.Defuzzification(0.60))
			{
				//Pass to hear state: The enemy hears something !!
				AlfredFSM->GoToNewState(EEnemyState::Hearing);
				//AlfredFSM->RunActionOfCurrentState();
				//@todo Move npc movement in Behavior Tree. 
				const FVector newLocationToReach = EnemyData.PlayerCharacter->GetActorLocation();
				ControllerNPC->MoveToLocation(newLocationToReach, 100.0f);
			}
		}
	}
}

void UAlfred::StopHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int otherBodyIndex)
{
	if (otherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(otherActor);
		if (MainCharacter)
		{
			bIsPlayerInTheHearingSphere = false;
		}
	}
}

