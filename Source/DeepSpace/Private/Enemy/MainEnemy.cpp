// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainEnemy.h"

#include "Materials/MaterialInstanceDynamic.h"


// Sets default values
AMainEnemy::AMainEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AlfredAI = CreateDefaultSubobject<UAlfred>(TEXT("AlfredAI"));
	AlfredFSM = CreateDefaultSubobject<UAlfredFSM>(TEXT("AlfredFSM"));
	ViewBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ViewBox"));
	ViewBox->SetupAttachment(RootComponent);
	HearingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HearingSphere"));
	HearingSphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMainEnemy::BeginPlay()
{
	Super::BeginPlay();
	CalmBT = NewObject<UCalmBT>();
	AttackBT = NewObject<UAttackBT>();
	WarningBT = NewObject<UWarningBT>();
	NoticeSomethingBT = NewObject<UNoticeSomethingBT>();
	AlfredAI->InitAI(CalmBT, AttackBT, WarningBT, NoticeSomethingBT, AlfredFSM, this, typeDamage);
	OnTakeAnyDamage.AddDynamic(this, &AMainEnemy::TakeDamageFromEnemy);
	state = AnimState::Walk;

	SetupBones();
}
void AMainEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AlfredAI->EnemyData.CharactersSeen.Empty();
	AlfredAI->EnemyData.CharactersHeard.Empty();
	AlfredAI->EnemyData.bonesOfCharacter.Empty();
}

void AMainEnemy::ResetDeath(AMainEnemy* Enemy)
{
	AlfredAI->ResetAfterDestroy(Enemy);
}


// Called every frame
void AMainEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FVector> AMainEnemy::GetCharacterBones()
{
	return Super::GetCharacterBones();
}

// Called to bind functionality to input
void AMainEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainEnemy::TakeDamageFromEnemy(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
	AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DamageCauser->GetActorLabel());
	TObjectPtr<AMainEnemy> EnemyThatAttack = Cast<AMainEnemy>(DamageCauser);
	if(EnemyThatAttack)
	{
		AlfredAI->EnemyData.CharactersHeard.Empty();
		AlfredAI->EnemyData.CharactersSeen.Empty();
		AlfredAI->EnemyData.CharactersSeen.AddUnique(EnemyThatAttack);
		AlfredAI->EnemyData.CharactersSeen.AddUnique(EnemyThatAttack);
	}
	health -= Damage;
	if (health <= 0.0f)
	{
		if (EnemyThatAttack)
		{
			EnemyThatAttack->AlfredAI->ResetAfterDestroy(this);
			for (int i = 0; i < EnemyThatAttack->AlfredAI->SameSideEnemy.Num(); i++)
			{
				AMainEnemy* currentEnemy = EnemyThatAttack->AlfredAI->SameSideEnemy[i];
				if(IsValid(currentEnemy))
				{
					currentEnemy->AlfredAI->ResetAfterDestroy(this);
				}
				else
				{
					EnemyThatAttack->AlfredAI->SameSideEnemy.Remove(currentEnemy);
				}
			}
			EnemyThatAttack->AlfredAI->isAttacked = false;
		}
		Destroy();
	}


}




