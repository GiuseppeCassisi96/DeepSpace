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


/*SENSORY FUNCTIONS : This type of function are responsible of all sensing task.
 *When a hostile entity will be added to the lists means that she was perceived.
 *When a hostile entity will be removed from the lists means that she was forgot 
 */


void UAlfred::StartVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults)
{
	if (otherActor)
	{
		//First of all I check if the owner of this component is an ally or not
		if (owner->Chartype == CharacterType::Ally)
		{
			//I check if the AMainEnemy reference is equal to owner or is another ally
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainEnemy)
			{
				if (MainEnemy == owner)
					return;
				if (MainEnemy->Chartype == CharacterType::Ally)
					return;
				EnemyData.CharactersSeen.AddUnique(MainEnemy);

			}
		}
		else
		{
			//I check if the actor perceived is a main character or a main enemy 
			AMainCharacter* MainCharacter = Cast<AMainCharacter>(otherActor);
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainCharacter)
			{
				EnemyData.CharactersSeen.AddUnique(MainCharacter);
			}
			if (MainEnemy && MainEnemy->Chartype != CharacterType::Enemy)
			{
				EnemyData.CharactersSeen.AddUnique(MainEnemy);
			}
		}
	}
}

void UAlfred::StopVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int otherBodyIndex)
{
	if (otherActor)
	{
		if (owner->Chartype == CharacterType::Ally)
		{
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainEnemy)
			{
				if (MainEnemy->Chartype == CharacterType::Player)
					return;
				EnemyData.CharactersSeen.Remove(MainEnemy);
			}
		}
		else
		{
			AMainCharacter* MainCharacter = Cast<AMainCharacter>(otherActor);
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainCharacter)
			{
				if (MainCharacter->Chartype == CharacterType::Enemy)
					return;
				EnemyData.CharactersSeen.Remove(MainCharacter);
			}

			if (MainEnemy && MainEnemy->Chartype != CharacterType::Enemy)
			{
				EnemyData.CharactersSeen.Remove(MainEnemy);
			}
		}
	}
}

void UAlfred::StartHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults)
{
	if (otherActor)
	{
		if (owner->Chartype == CharacterType::Ally)
		{
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainEnemy)
			{
				if (MainEnemy->Chartype == CharacterType::Player)
					return;
				if (MainEnemy == owner)
					return;
				if (MainEnemy->Chartype == CharacterType::Ally)
					return;
				EnemyData.CharactersHeard.AddUnique(MainEnemy);
			}
		}
		else
		{

			AMainCharacter* MainCharacter = Cast<AMainCharacter>(otherActor);
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainCharacter)
			{
				EnemyData.CharactersHeard.AddUnique(MainCharacter);
			}
			if (MainEnemy && MainEnemy->Chartype != CharacterType::Enemy)
			{
				EnemyData.CharactersHeard.AddUnique(MainEnemy);
			}
		}

		AThrowableItem* ThrowableItem = Cast<AThrowableItem>(otherActor);
		if (ThrowableItem)
		{
			if (!ThrowableItem->OnThrowableItemEvent.IsBound())
				ThrowableItem->OnThrowableItemEvent.AddDynamic(this, &UAlfred::ItemHitNearEnemy);
		}

	}
}

void UAlfred::StopHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int otherBodyIndex)
{
	if (otherActor)
	{
		if (owner->Chartype == CharacterType::Ally)
		{
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainEnemy)
			{
				if (MainEnemy->Chartype == CharacterType::Player)
					return;
				EnemyData.CharactersHeard.Remove(MainEnemy);
			}
		}
		else
		{
			AMainCharacter* MainCharacter = Cast<AMainCharacter>(otherActor);
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainCharacter)
			{
				EnemyData.CharactersHeard.Remove(MainCharacter);
			}
			if (MainEnemy && MainEnemy->Chartype != CharacterType::Enemy)
			{
				EnemyData.CharactersHeard.Remove(MainEnemy);
			}
		}
	}
}

void UAlfred::ItemHitNearEnemy(FVector hitLocation, float itemNoisiness)
{
	/*The 'pathCost' indicates the noise that the enemy perceives. This noise use a pathfinding system in order
	 *to take into account the environment of the level. Less is the 'pathCost' value more is the noise perceived.
	 */
	float pathCost;
	if (owner->Chartype == CharacterType::Ally)
		return;
	if (owner->AlfredFSM->GetCurrentState() != EEnemyState::Attack)
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
			owner->AlfredFSM->GoToNewState(EEnemyState::NoticeSomething);
			Cast<UNoticeSomethingBT>(owner->AlfredFSM->GetStates()[EEnemyState::NoticeSomething])->SourceLocation = hitLocation;
			Cast<UWarningBT>(owner->AlfredFSM->GetStates()[EEnemyState::Warning])->Location = hitLocation;
			owner->AlfredFSM->RunActionOfCurrentState();
			bHasNoticeSomething = true;
		}
	}

}

void UAlfred::EnemyView()
{
	
	if (!EnemyData.CharactersSeen.IsEmpty())
	{
		//Here I get the positions of character bones
		EnemyData.bonesOfCharacter = EnemyData.CharactersSeen[0]->GetCharacterBones();
		const FVector npcEyesLocation = owner->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f);
		//Then I cast a number of ray equal to the number of bones locations that I'm taking into account
		for (auto boneLocation : EnemyData.bonesOfCharacter)
		{

			UKismetSystemLibrary::LineTraceSingle(this, npcEyesLocation,
				boneLocation, UEngineTypes::ConvertToTraceType(ECC_Camera), true,
				actorToIgnore, EDrawDebugTrace::None, HitResult, true);
			//I keep track of the number of the rays that hit the bones
			if (Cast<ABaseMain>(HitResult.GetActor()))
			{
				seeCount++;
			}
		}


		//ACTIONS BASED ON SEE COUNT
		//I map the seeCount value into a membership value of my fuzzy set between 0 and 1
		SeeSet.Fuzzification(EnemyData.bonesOfCharacter.Num(), seeCount);

		if (SeeSet.Defuzzification(0.65f))
		{
			//I'm setting the location also for the 'UWarningBT' for future purposes  
			Cast<UWarningBT>(owner->AlfredFSM->GetStates()[EEnemyState::Warning])->Location = EnemyData.CharactersSeen[0]->GetActorLocation();
			Cast<UAttackBT>(owner->AlfredFSM->GetStates()[EEnemyState::Attack])->playerRefBT = EnemyData.CharactersSeen[0];
			//Pass to attack state: The enemy sees you !!
			owner->AlfredFSM->GoToNewState(EEnemyState::Attack);
			owner->AlfredFSM->RunActionOfCurrentState();
			bHasSeen = true;
			owner->GetWorldTimerManager().ClearTimer(GoToCalmTimer);
		}
		else if (SeeSet.Defuzzification(0.39f))
		{
			if (owner->AlfredFSM->GetCurrentState() != EEnemyState::Attack)
			{
				//I'm setting the location also for the 'UWarningBT' for future purposes  
				Cast<UWarningBT>(owner->AlfredFSM->GetStates()[EEnemyState::Warning])->Location = EnemyData.CharactersSeen[0]->GetActorLocation();
				Cast<UNoticeSomethingBT>(owner->AlfredFSM->GetStates()[EEnemyState::NoticeSomething])->SourceLocation = EnemyData.CharactersSeen[0]->GetActorLocation();
				//Pass to NoticeSomething state: The enemy has seen something and getting worried
				owner->AlfredFSM->GoToNewState(EEnemyState::NoticeSomething);
				owner->AlfredFSM->RunActionOfCurrentState();
				bHasNoticeSomething = true;
			}
		}
		seeCount = 0;
	}
}


void UAlfred::EnemyHearing()
{
	if (!EnemyData.CharactersHeard.IsEmpty())
	{
		/*The 'pathCost' indicates the noise that the enemy perceives. This noise use a pathfinding system in order
		*to take into account the environment of the level. Less is the 'pathCost' value more is the noise perceived.
		*/
		float pathCost;
		if (EnemyData.CharactersHeard[0]->state != AnimState::Crouch &&
			owner->AlfredFSM->GetCurrentState() != EEnemyState::Attack)
		{

			navSys->GetPathCost(this, owner->GetActorLocation(),
				EnemyData.CharactersHeard[0]->GetActorLocation(), pathCost);
			float reduceFactor = static_cast<float>(EnemyData.CharactersHeard[0]->state);

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
				GEngine->AddOnScreenDebugMessage(-1, 3.0f,
					FColor::Red, TEXT("HEAR"));
				//Pass to NoticeSomething state: The enemy hears something !!
				Cast<UWarningBT>(owner->AlfredFSM->GetStates()[EEnemyState::Warning])->Location = EnemyData.CharactersHeard[0]->GetActorLocation();
				Cast<UNoticeSomethingBT>(owner->AlfredFSM->GetStates()[EEnemyState::NoticeSomething])->SourceLocation = EnemyData.CharactersHeard[0]->GetActorLocation();
				owner->AlfredFSM->GoToNewState(EEnemyState::NoticeSomething);
				owner->AlfredFSM->RunActionOfCurrentState();
				bHasNoticeSomething = true;
			}
		}
	}
}

//SENSORY FUNCTIONS:-----------------------------------------------------------------------------------------

//INIT AI FUNCTION: This function is responsible to initialize the AI.
void UAlfred::InitAI(TObjectPtr<UCalmBT> CalmBT, TObjectPtr<UAttackBT> AttackBT, TObjectPtr<
	UWarningBT> WarningBT, TObjectPtr<UNoticeSomethingBT> NoticeSomethingBT,
	TObjectPtr<UAlfredFSM> AlfredFSM, TObjectPtr<ACharacter> enemy, TSubclassOf<UDamageType> typeDamage)
{
	owner = Cast<AMainEnemy>(enemy);

	//I bind the sensory functions with the box and sphere delegates  
	owner->ViewBox->OnComponentBeginOverlap.AddDynamic(this, &UAlfred::StartVisualSensors);
	owner->ViewBox->OnComponentEndOverlap.AddDynamic(this, &UAlfred::StopVisualSensors);
	owner->HearingSphere->OnComponentBeginOverlap.AddDynamic(this, &UAlfred::StartHearSensors);
	owner->HearingSphere->OnComponentEndOverlap.AddDynamic(this, &UAlfred::StopHearSensors);

	navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	ControllerNPC = Cast<AAIController>(owner->GetController());
	ControllerNPC->ReceiveMoveCompleted.AddDynamic(this, &UAlfred::NPCReachesTheLocation);

	//Initialize and add to the FSM the Behavior trees
	CalmBT->InitTree(enemy, navSys);
	AlfredFSM->GetStates().Add(EEnemyState::Calm, CalmBT);
	AttackBT->InitTree(enemy, navSys, nullptr, typeDamage);
	AlfredFSM->GetStates().Add(EEnemyState::Attack, AttackBT);
	WarningBT->InitTree(owner, navSys);
	AlfredFSM->GetStates().Add(EEnemyState::Warning, WarningBT);
	NoticeSomethingBT->InitTree(owner, navSys, FVector::Zero());
	AlfredFSM->GetStates().Add(EEnemyState::NoticeSomething, NoticeSomethingBT);
	//Run the default state (Calm state)
	AlfredFSM->RunActionOfCurrentState();
}
//INIT AI FUNCTION:-----------------------------------------------------------------------------------------

//EVENT FUNCTIONS: These functions are event functions that will are invoked when a specific situation occurs
void UAlfred::NPCReachesTheLocation(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if ((bHasSeen || bHasNoticeSomething) && Result == EPathFollowingResult::Success && !SeeSet.Defuzzification(0.65f))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f,
			FColor::Red, TEXT("WARNING AFTER SEEN"));
		//Pass to warning state: He has noticed something and now he getting worried
		owner->AlfredFSM->GoToNewState(EEnemyState::Warning);
		owner->AlfredFSM->RunActionOfCurrentState();
		owner->GetWorldTimerManager().SetTimer(GoToCalmTimer, this,
			&UAlfred::GoToCalmState, 60.0f);
		bHasSeen = false;
		bHasNoticeSomething = false;
	}
}

void UAlfred::GoToCalmState()
{

	UE_LOG(LogTemp, Error, TEXT("GoToCalmState"));
	owner->AlfredFSM->GoToNewState(EEnemyState::Calm);
	owner->AlfredFSM->RunActionOfCurrentState();
}
//EVENT FUNCTIONS:-----------------------------------------------------------------------------------------



// Called when the game starts
void UAlfred::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UAlfred::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	EnemyView(); //RUN THE VIEW SENSOR
	EnemyHearing(); //RUN THE HEAR SENSOR
}

void UAlfred::ResetAfterDestroy(AMainEnemy* Enemy)
{
	//Remove the enemy destroyed 
	int n1 = EnemyData.CharactersHeard.Remove(Enemy);
	int n2 = EnemyData.CharactersSeen.Remove(Enemy);
	if(n1 + n2 > 0 && owner->AlfredFSM->GetCurrentState() == EEnemyState::Attack)
	{
		bHasSeen = false;
		bHasNoticeSomething = false;
		GoToCalmState();
	}
	
}








