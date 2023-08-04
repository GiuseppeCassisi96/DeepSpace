// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainEnemy.h"

#include "AI/AlfredSensing.h"
#include "Materials/MaterialInstanceDynamic.h"


// Sets default values
AMainEnemy::AMainEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AlfredAI = CreateDefaultSubobject<UAlfred>(TEXT("AlfredAI"));
	ViewBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ViewBox"));
	ViewBox->SetupAttachment(RootComponent);
	HearingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HearingSphere"));
	HearingSphere->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void AMainEnemy::BeginPlay()
{
	Super::BeginPlay();
	//AIController creation 
	TSubclassOf<AAlfredAIController> AlfredControllerClass;
	AlfredControllerClass = AAlfredAIController::StaticClass();
	ControllerNPC = GetWorld()->SpawnActor<AAlfredAIController>(AlfredControllerClass);
	ControllerNPC->SetAlfredController(this);
	ControllerNPC->Possess(this);

	AlfredAI->InitAI();
	OnTakeAnyDamage.AddDynamic(this, &AMainEnemy::TakeDamageFromEnemy);
	state = AnimState::Walk;
	MaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0),
		NULL);
	if(Chartype == ECharacterType::Ally)
	{
		MaterialInstance->SetVectorParameterValue(TEXT("ColorMat"), FLinearColor::Blue);
		GetMesh()->SetMaterial(0, MaterialInstance);
	}
	else
	{
		MaterialInstance->SetVectorParameterValue(TEXT("ColorMat"), FLinearColor::Red);
		GetMesh()->SetMaterial(0, MaterialInstance);
	}
	
	SetupBones();
}
void AMainEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AlfredAI->AlfredSensing->EnemyData.CharactersSeen.Empty();
	AlfredAI->AlfredSensing->EnemyData.CharactersHeard.Empty();
	AlfredAI->AlfredSensing->EnemyData.bonesOfCharacter.Empty();
}

void AMainEnemy::ResetDeath(AMainEnemy* Enemy)
{
	AlfredAI->GoToCalmStateAfterDestroy(Enemy);
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
	TObjectPtr<AMainEnemy> EnemyThatAttack = Cast<AMainEnemy>(DamageCauser);
	if(EnemyThatAttack)
	{
		AlfredAI->AlfredSensing->EnemyData.CharactersHeard.Empty();
		AlfredAI->AlfredSensing->EnemyData.CharactersSeen.Empty();
		AlfredAI->AlfredSensing->EnemyData.CharactersSeen.AddUnique(EnemyThatAttack);
		AlfredAI->AlfredSensing->EnemyData.CharactersSeen.AddUnique(EnemyThatAttack);
	}
	health -= Damage;
	if (health <= 0.0f)
	{
		if (EnemyThatAttack)
		{
			EnemyThatAttack->AlfredAI->GoToCalmStateAfterDestroy(this);
			for (int i = 0; i < EnemyThatAttack->AlfredAI->AlfredSensing->EnemyData.SameSideEntity.Num(); i++)
			{
				AMainEnemy* currentEnemy = EnemyThatAttack->AlfredAI->AlfredSensing->EnemyData.SameSideEntity[i];
				if(IsValid(currentEnemy))
				{
					currentEnemy->AlfredAI->GoToCalmStateAfterDestroy(this);
				}
				else
				{
					EnemyThatAttack->AlfredAI->AlfredSensing->EnemyData.SameSideEntity.Remove(currentEnemy);
				}
			}
			EnemyThatAttack->AlfredAI->isAttacked = false;
		}
		Destroy();
	}


}




