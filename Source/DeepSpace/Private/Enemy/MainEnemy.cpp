// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainEnemy.h"



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
	AlfredAI->InitAI(CalmBT, AttackBT, AlfredFSM, this, typeDamage,WarningBT, NoticeSomethingBT);
	OnTakeAnyDamage.AddDynamic(this, &AMainEnemy::TakeDamageFromEnemy);
	state = AnimState::Walk;
}

void AMainEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AlfredAI->EnemyData.CharactersSeen.Empty();
	AlfredAI->EnemyData.CharactersHeard.Empty();
	AlfredAI->EnemyData.bonesOfPlayer.Empty();
}

void AMainEnemy::ResetDeath()
{
	AlfredAI->ResetAfterDestroy();
}


// Called every frame
void AMainEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	AlfredAI->SetIsAttacked(EnemyThatAttack);
	health -= Damage;
	if(health <= 0.0f)
	{
		Destroy();
		TimerDelegate.BindUFunction(EnemyThatAttack, "ResetDeath");
		//Clean Dead enemy data and reset to default state
		GetWorldTimerManager().SetTimer(Handle, TimerDelegate, 0.5f, false);
	}
}




