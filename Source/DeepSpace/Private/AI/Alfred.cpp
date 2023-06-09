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
	}
}

// Called every frame
void UAlfred::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	EnemyView();
	EnemyHearing();
	owner->AlfredFSM->CheckTreeStatus();
}

void UAlfred::SetIsAttacked(TObjectPtr<AMainEnemy> EnemyThatAttack)
{
	
	if(!isAlly && !isAttacked)
	{
		EnemyData.CharactersHeard.Empty();
		EnemyData.CharactersSeen.Empty();
		EnemyData.CharactersSeen.Add(EnemyThatAttack);
		EnemyData.CharactersHeard.Add(EnemyThatAttack);
		Cast<UAttackBT>(owner->AlfredFSM->GetStates()[EEnemyState::Attack])->playerRefBT = EnemyData.CharactersSeen[0];
		owner->AlfredFSM->GoToNewState(EEnemyState::Attack);
		owner->AlfredFSM->RunActionOfCurrentState();
		isAttacked = true;
	}

}

void UAlfred::ResetAfterDestroy()
{
	EnemyData.CharactersHeard.Empty();
	EnemyData.CharactersSeen.Empty();
	EnemyData.bonesOfPlayer.Empty();
	isAttacked = false;
	bHasSeen = false;
	GoToCalmState();
}

void UAlfred::NPCReachsTheLocation(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if ((bHasSeen || bHasNoticeSomething) && Result == EPathFollowingResult::Success && !SeeSet.Defuzzification(0.65f))
	{
		//Pass to warning state: He has seen the player, but now doesn't see him anymore
		GEngine->AddOnScreenDebugMessage(-1, 2.0f,
			FColor::Red, TEXT("WARNING AFTER SEEN"));
		owner->AlfredFSM->GoToNewState(EEnemyState::Warning);
		owner->AlfredFSM->RunActionOfCurrentState();
		owner->GetWorldTimerManager().SetTimer(GoToCalmTimer, this, 
			&UAlfred::GoToCalmState, 60.0f);
		bHasSeen = false;
		bHasNoticeSomething = false;
	}
}

void UAlfred::ItemHitNearEnemy(FVector hitLocation, float itemNoisiness)
{
	/*The 'pathCost' indicates the noise that the enemy perceives. This noise use a pathfinding system in order
	 *to take into account the environment of the level. Less is the 'pathCost' value more is the noise perceived. 
	 */
	float pathCost;
	if(isAlly)
		return;
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
			owner->AlfredFSM->GoToNewState(EEnemyState::NoticeSomething);
			Cast<UNoticeSomethingBT>(owner->AlfredFSM->GetStates()[EEnemyState::NoticeSomething])->SourceLocation = hitLocation;
			Cast<UWarningBT>(owner->AlfredFSM->GetStates()[EEnemyState::Warning])->Location = hitLocation;
			owner->AlfredFSM->RunActionOfCurrentState();
			bHasNoticeSomething = true;
		}
	}
	
}

void UAlfred::GoToCalmState()
{
	owner->AlfredFSM->GoToNewState(EEnemyState::Calm);
	owner->AlfredFSM->RunActionOfCurrentState();
}

void UAlfred::InitAI(TObjectPtr<UCalmBT> CalmBT, TObjectPtr<UAttackBT> AttackBT,
                     TObjectPtr<UAlfredFSM> AlfredFSM, TObjectPtr<ACharacter> enemy, TSubclassOf<UDamageType> typeDamage, TObjectPtr<
	                     UWarningBT> WarningBT, TObjectPtr<UNoticeSomethingBT> NoticeSomethingBT)
{
	UNavigationSystemV1* NavigationSystemV1 = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	CalmBT->InitTree(enemy, NavigationSystemV1);
	AlfredFSM->GetStates().Add(EEnemyState::Calm, CalmBT);
	AttackBT->InitTree(enemy, NavigationSystemV1, nullptr, typeDamage);
	AlfredFSM->GetStates().Add(EEnemyState::Attack, AttackBT);
	WarningBT->InitTree(owner, NavigationSystemV1);
	AlfredFSM->GetStates().Add(EEnemyState::Warning, WarningBT);
	NoticeSomethingBT->InitTree(owner, NavigationSystemV1, FVector::Zero());
	AlfredFSM->GetStates().Add(EEnemyState::NoticeSomething, NoticeSomethingBT);
	AlfredFSM->RunActionOfCurrentState();
}


void UAlfred::StartVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
                                 int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults)
{
	if (otherActor)
	{
		if(isAlly || isAttacked)
		{
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainEnemy)
			{
				if (MainEnemy->Chartype == CharacterType::Player)
					return;
				if(MainEnemy == owner)
					return;
				EnemyData.CharactersSeen.Add(MainEnemy);
			}
		}
		else
		{
			AMainCharacter* MainCharacter = Cast<AMainCharacter>(otherActor);
			if (MainCharacter)
			{
				if (MainCharacter->Chartype == CharacterType::Enemy)
					return;
				EnemyData.CharactersSeen.Add(MainCharacter);
			}
		}
	}
}

void UAlfred::StopVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
	int otherBodyIndex)
{
	if (otherActor)
	{
		if (isAlly || isAttacked)
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
			if (MainCharacter)
			{
				if (MainCharacter->Chartype == CharacterType::Enemy)
					return;
				EnemyData.CharactersSeen.Remove(MainCharacter);
			}
		}
	}
}

void UAlfred::StartHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults)
{
	if (otherActor)
	{
		
		//When the player enters in the hear sphere
		if (isAlly || isAttacked)
		{
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainEnemy)
			{
				if (MainEnemy->Chartype == CharacterType::Player)
					return;
				if (MainEnemy == owner)
					return;
				EnemyData.CharactersHeard.Add(MainEnemy);
			}
		}
		else
		{
			
			AMainCharacter* MainCharacter = Cast<AMainCharacter>(otherActor);
			if (MainCharacter)
			{
				if (MainCharacter->Chartype == CharacterType::Enemy)
					return;
				EnemyData.CharactersHeard.Add(MainCharacter);
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
		if (isAlly || isAttacked)
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
			if (MainCharacter)
			{
				if (MainCharacter->Chartype == CharacterType::Enemy)
					return;
				EnemyData.CharactersHeard.Remove(MainCharacter);
			}
		}
	}
}

void UAlfred::EnemyView()
{
	 //CAST RAY
	if(!EnemyData.CharactersSeen.IsEmpty())
	{
		EnemyData.bonesOfPlayer = EnemyData.CharactersSeen[0]->GetCharacterBones();
		const FVector npcEyesLocation = owner->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f);
		for(auto boneLocation: EnemyData.bonesOfPlayer)
		{
			UKismetSystemLibrary::LineTraceSingle(this, npcEyesLocation,
				boneLocation, UEngineTypes::ConvertToTraceType(ECC_Camera), true,
				actorToIgnore, EDrawDebugTrace::None, HitResult, true);
			if (Cast<ABaseMain>(HitResult.GetActor()))
			{
				seeCount++;
			}
		}
	

		//ACTIONS BASED ON SEE COUNT
		//I map the seeCount value into a membership value of my fuzzy set between 0 and 1
		SeeSet.Fuzzification(EnemyData.bonesOfPlayer.Num(), seeCount);

		if (SeeSet.Defuzzification( 0.65f))
		{
			Cast<UAttackBT>(owner->AlfredFSM->GetStates()[EEnemyState::Attack])->playerRefBT = EnemyData.CharactersSeen[0];
			//Pass to attack state: The enemy sees you !!
			owner->AlfredFSM->GoToNewState(EEnemyState::Attack);
			ETaskExeState state = owner->AlfredFSM->RunActionOfCurrentState();
			bHasSeen = true;
			owner->GetWorldTimerManager().ClearTimer(GoToCalmTimer);
		}
		else if (SeeSet.Defuzzification( 0.39f))
		{
			if (!bHasSeen)
			{
				Cast<UWarningBT>(owner->AlfredFSM->GetStates()[EEnemyState::Warning])->Location = EnemyData.CharactersSeen[0]->GetActorLocation();
				//Pass to warning state: The enemy has seen something and getting worried
				Cast<UNoticeSomethingBT>(owner->AlfredFSM->GetStates()[EEnemyState::NoticeSomething])->SourceLocation = EnemyData.CharactersSeen[0]->GetActorLocation();
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
	if(!EnemyData.CharactersHeard.IsEmpty())
	{
		/*The 'pathCost' indicates the noise that the enemy perceives. This noise use a pathfinding system in order
	    *to take into account the environment of the level. Less is the 'pathCost' value more is the noise perceived.
	    */
		float pathCost;
		//UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (bHasSeen ? TEXT("true") : TEXT("false")));
		if(EnemyData.CharactersHeard[0]->state != AnimState::Crouch && !bHasSeen)
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
				//Pass to hear state: The enemy hears something !!
				Cast<UWarningBT>(owner->AlfredFSM->GetStates()[EEnemyState::Warning])->Location = EnemyData.CharactersHeard[0]->GetActorLocation();
				Cast<UNoticeSomethingBT>(owner->AlfredFSM->GetStates()[EEnemyState::NoticeSomething])->SourceLocation = EnemyData.CharactersHeard[0]->GetActorLocation();
				owner->AlfredFSM->GoToNewState(EEnemyState::NoticeSomething);
				owner->AlfredFSM->RunActionOfCurrentState();
				bHasNoticeSomething = true;
			}
		}
	}
}

