// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH2_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

	// Getters for Health and MaxHealth
	UFUNCTION(BlueprintPure)
	float GetHealth() const { return Health; }
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const { return MaxHealth; }
	float HealthPercentage;

	FORCEINLINE float GetHealthPercentage() const { return Health / MaxHealth; }

	void RecieveDamage(float Damage);


	void AddGold(int32 Amount) { Gold += Amount; }

	int32 GetGold() const { return Gold; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE bool IsAlive() const { return Health > 0; }


private:
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	int32 Gold = 0;

	// Current health
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Health = 100;


		
};
