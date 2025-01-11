// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"


class UGeometryCollectionComponent;

UCLASS()
class SLASH2_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	virtual void Tick(float DeltaTime) override;
	
	// Sets default values for this actor's properties
	ABreakableActor();

	virtual void GetHit_Implementation(const FVector& ImpactPoint, const AWeapon* Weapon) override; 
	virtual void Die_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AItem>> ItemsToSpawn;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATreasure> TreasureClass;
};
