// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AlfredSensing.h"


// Sets default values for this component's properties
UAlfredSensing::UAlfredSensing()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	owner = Cast<AMainEnemy>(GetOwner());
	
	// ...
}

void UAlfredSensing::StartVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults)
{
	if (otherActor)
	{
		//First of all I check if the owner of this component is an ally or not
		if (owner->Chartype == ECharacterType::Ally)
		{
			//I check if the AMainEnemy reference is equal to owner or is another ally
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainEnemy)
			{
				if (MainEnemy == owner)
					return;
				if (MainEnemy->Chartype == ECharacterType::Player)
					return;
				if (MainEnemy->Chartype == ECharacterType::Ally)
				{
					EnemyData.SameSideEntity.AddUnique(MainEnemy);
					return;
				}
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
			if (MainEnemy)
			{
				if (MainEnemy->Chartype == ECharacterType::Enemy)
				{
					EnemyData.SameSideEntity.AddUnique(MainEnemy);
					return;
				}
				EnemyData.CharactersSeen.AddUnique(MainEnemy);
			}
		}
	}
}

void UAlfredSensing::StopVisualSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int otherBodyIndex)
{
	if (otherActor)
	{
		if (owner->Chartype == ECharacterType::Ally)
		{
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainEnemy)
			{
				if (MainEnemy->Chartype == ECharacterType::Player)
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
				if (MainCharacter->Chartype == ECharacterType::Enemy)
					return;
				EnemyData.CharactersSeen.Remove(MainCharacter);
			}

			if (MainEnemy && MainEnemy->Chartype != ECharacterType::Enemy)
			{
				EnemyData.CharactersSeen.Remove(MainEnemy);
			}
		}
	}
}

void UAlfredSensing::StartHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepResults)
{
	if (otherActor)
	{
		if (owner->Chartype == ECharacterType::Ally)
		{
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainEnemy)
			{
				if (MainEnemy->Chartype == ECharacterType::Player)
					return;
				if (MainEnemy == owner)
					return;
				if (MainEnemy->Chartype == ECharacterType::Ally)
				{
					EnemyData.SameSideEntity.AddUnique(MainEnemy);
					return;
				}
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
			if (MainEnemy)
			{
				if (MainEnemy->Chartype == ECharacterType::Enemy)
				{
					EnemyData.SameSideEntity.AddUnique(MainEnemy);
					return;
				}
				EnemyData.CharactersSeen.AddUnique(MainEnemy);
			}
		}

		AThrowableItem* ThrowableItem = Cast<AThrowableItem>(otherActor);
		if (ThrowableItem)
		{
			if (!ThrowableItem->OnThrowableItemEvent.IsBound())
				ThrowableItem->OnThrowableItemEvent.AddDynamic(this, &UAlfredSensing::ItemHitNearEnemy);
		}

	}
}

void UAlfredSensing::StopHearSensors(UPrimitiveComponent* OverlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int otherBodyIndex)
{
	if (otherActor)
	{
		if (owner->Chartype == ECharacterType::Ally)
		{
			AMainEnemy* MainEnemy = Cast<AMainEnemy>(otherActor);
			if (MainEnemy)
			{
				if (MainEnemy->Chartype == ECharacterType::Player)
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
			if (MainEnemy && MainEnemy->Chartype != ECharacterType::Enemy)
			{
				EnemyData.CharactersHeard.Remove(MainEnemy);
			}
		}
		AThrowableItem* ThrowableItem = Cast<AThrowableItem>(otherActor);
		if (ThrowableItem)
		{
			if (ThrowableItem->OnThrowableItemEvent.IsBound())
				ThrowableItem->OnThrowableItemEvent.RemoveDynamic(this, &UAlfredSensing::ItemHitNearEnemy);
		}
	}
}

void UAlfredSensing::ItemHitNearEnemy(FVector hitLocation, float itemNoisiness)
{
	/*The 'pathCost' indicates the noise that the enemy perceives. This noise use a pathfinding system in order
	 *to take into account the environment of the level. Less is the 'pathCost' value more is the noise perceived.
	 */
	float pathCost;
	if (owner->Chartype == ECharacterType::Ally || 
		owner->AlfredAI->AlfredFSM->GetCurrentState() == EEnemyState::Attack)
		return;

	owner->ControllerNPC->GetNavSys()->GetPathCost(this, owner->GetActorLocation(),
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
		owner->GetWorldTimerManager().ClearTimer(owner->AlfredAI->GoToCalmTimer);
		//Pass to hear state: The enemy hears something !!
		owner->AlfredAI->AlfredFSM->GoToNewState(EEnemyState::NoticeSomething);
		Cast<UNoticeSomethingBT>(owner->AlfredAI->AlfredFSM->GetFSM()[EEnemyState::NoticeSomething]->stateAction)->SourceLocation = hitLocation;
		Cast<UWarningBT>(owner->AlfredAI->AlfredFSM->GetFSM()[EEnemyState::Warning]->stateAction)->Location = hitLocation;
		owner->AlfredAI->AlfredFSM->RunActionOfCurrentState();
	}
}

void UAlfredSensing::EnemyView()
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
		SeeSet.Fuzzification(static_cast<float>(EnemyData.bonesOfCharacter.Num()), seeCount);

		if (SeeSet.Defuzzification(0.65f))
		{
			//I'm setting the location also for the 'UWarningBT' for future purposes  
			Cast<UWarningBT>(owner->AlfredAI->AlfredFSM->GetFSM()[EEnemyState::Warning]->stateAction)->Location = EnemyData.CharactersSeen[0]->GetActorLocation();
			Cast<UAttackBT>(owner->AlfredAI->AlfredFSM->GetFSM()[EEnemyState::Attack]->stateAction)->playerRefBT = EnemyData.CharactersSeen[0];
			Cast<UAttackBT>(owner->AlfredAI->AlfredFSM->GetFSM()[EEnemyState::Attack]->stateAction)->typeDamage = owner->typeDamage;
			//Pass to attack state: The enemy sees you !!
			owner->AlfredAI->AlfredFSM->GoToNewState(EEnemyState::Attack);
			owner->AlfredAI->AlfredFSM->RunActionOfCurrentState();
			owner->GetWorldTimerManager().ClearTimer(owner->AlfredAI->GoToCalmTimer);
		}
		else if (SeeSet.Defuzzification(0.39f))
		{
			if (owner->AlfredAI->AlfredFSM->GetCurrentState() != EEnemyState::Attack)
			{
				//I'm setting the location also for the 'UWarningBT' for future purposes  
				Cast<UWarningBT>(owner->AlfredAI->AlfredFSM->GetFSM()[EEnemyState::Warning]->stateAction)->Location = EnemyData.CharactersSeen[0]->GetActorLocation();
				Cast<UNoticeSomethingBT>(owner->AlfredAI->AlfredFSM->GetFSM()[EEnemyState::NoticeSomething]->stateAction)->SourceLocation = EnemyData.CharactersSeen[0]->GetActorLocation();
				//Pass to NoticeSomething state: The enemy has seen something and getting worried
				owner->AlfredAI->AlfredFSM->GoToNewState(EEnemyState::NoticeSomething);
				owner->AlfredAI->AlfredFSM->RunActionOfCurrentState();
			}
		}
		seeCount = 0;
	}
}

void UAlfredSensing::EnemyHearing()
{
	if (!EnemyData.CharactersHeard.IsEmpty())
	{
		/*The 'pathCost' indicates the nearness to the noise source. The auditory system use a pathfinding system
		 *in order to take into account the environment of the level. Less is the 'pathCost' value more is
		 *the noise perceived.
		*/
		float pathCost;
		if (owner->AlfredAI->AlfredFSM->GetCurrentState() != EEnemyState::Attack)
		{

			owner->ControllerNPC->GetNavSys()->GetPathCost(this, owner->GetActorLocation(),
				EnemyData.CharactersHeard[0]->GetActorLocation(), pathCost);
			float reduceFactor = static_cast<float>(EnemyData.CharactersHeard[0]->state);

			//I'm sure that the pathCost is between 0.0f and 1500.0f
			pathCost = std::clamp(pathCost - reduceFactor, 0.0f, 1500.0f);
			/*Less is the pathCost less is the membership value of NonHearSet and more is the
			 *membership value of HearSet, obtained as the negation of NonHearSet (1.0 - membValNonHearSet)*/
			NonHearSet.Fuzzification(1500.0f, pathCost);

			/*I obtain the HearSet simply denying the NonHearSet. When the membership value of NonHearSet is high
			 *the membership value of HearSet is low and vice versa. I overload the operator '!'*/
			NonHearSet.Set.membershipValue = !NonHearSet;
			/*Now we have the negated value of membership value of NonHearSet, so we have the membership value
			 * of HearSet. So for example if the membership value of NonHearSet is 0.10 the membership value
			 * of HearSet is 0.90 (1.0 - 0.10)*/
			if (NonHearSet.Defuzzification(0.60))
			{
				//Pass to NoticeSomething state: The enemy hears something !!
				Cast<UWarningBT>(owner->AlfredAI->AlfredFSM->GetFSM()[EEnemyState::Warning]->stateAction)->Location = EnemyData.CharactersHeard[0]->GetActorLocation();
				Cast<UNoticeSomethingBT>(owner->AlfredAI->AlfredFSM->GetFSM()[EEnemyState::NoticeSomething]->stateAction)->SourceLocation = EnemyData.CharactersHeard[0]->GetActorLocation();
				owner->AlfredAI->AlfredFSM->GoToNewState(EEnemyState::NoticeSomething);
				owner->AlfredAI->AlfredFSM->RunActionOfCurrentState();
			}
		}
	}
}

void UAlfredSensing::SetOwner(TObjectPtr<AMainEnemy> ownerComponent)
{
	owner = ownerComponent.Get();
}


