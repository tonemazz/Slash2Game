// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class USoundBase;

UCLASS()
class SLASH2_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Default constructor
	AItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Item Properties")
	USoundBase* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Behavior")
	float RotationSpeedX = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Behavior")
	float RotationSpeedY = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Behavior")
	float RotationSpeedZ = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Behavior")
	bool bRotate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template <typename T>
	T Avg(T First, T Second);

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Equipped = false;

private:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;








public:
	void SetEquipped() { Equipped = true; }
	void SetUneqiupped() { Equipped = false; }

	
};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
