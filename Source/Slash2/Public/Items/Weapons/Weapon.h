// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class SLASH2_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	
	
	TArray<AActor*> IgnoreActors; // Semi-permanent list of already-hit actors so there's no multi-hits on a single actor. Must be cleared after each attack.

	void EquipWeapon(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	UFUNCTION(BlueprintCallable)
	void SetAttackCollisionState(ECollisionEnabled::Type CollisionEnabled);
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	int32 Damage = 25;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

	UPROPERTY(EditAnywhere, Category = "Item Properties")
	UBoxComponent* WeaponBox;
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	AActor* Equipper;

};
