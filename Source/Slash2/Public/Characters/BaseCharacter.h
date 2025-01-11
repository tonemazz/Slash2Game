#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class USoundBase;
class UParticleSystem;

UCLASS()
class SLASH2_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Public Methods 
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable) void SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled);
	UAttributeComponent* GetAttributes();

	// Public Properties

protected:

	// BeginPlay
	virtual void BeginPlay() override; 
	
	// Behavior Functions
	virtual void Attack();
	UFUNCTION(BlueprintCallable) virtual void AttackEnd();

	// Hit & Damage Functions
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitParticles(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	virtual int32 PlayAttackMontage();
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	
	// Checker Functions
	virtual bool CanAttack();
	bool IsAlive();
	void DisableCapsule();
	void Ragdoll();

	// Protected Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) AWeapon* EquippedWeapon;
	UPROPERTY(EditDefaultsOnly, Category = Montages) UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages) UAnimMontage* HitReactMontage;
	UPROPERTY(EditAnywhere, Category = Combat) TArray<FName> AttackMontageSections;
	UPROPERTY(EditAnywhere, Category = Attributes) UAttributeComponent* Attributes;
	UPROPERTY(EditAnywhere, Category = Sound) USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = Sound) UParticleSystem* HitParticles;
	UPROPERTY(EditAnywhere) float DeathDespawnTime = 5.f;




private:
	// Private Methods


	// Private Properties

};
