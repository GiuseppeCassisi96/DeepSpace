// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ThrowableItem.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FThrowableItemEvent,FVector, hitLocation, float, itemNoisiness);
UCLASS()
class DEEPSPACE_API AThrowableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnGeneralHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(EditDefaultsOnly, Category="StaticMesh")
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditDefaultsOnly, Category= "ItemProperty")
	float itemNoisiness = 20.0f;
	bool bIsFirstHit;
	FThrowableItemEvent OnThrowableItemEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

