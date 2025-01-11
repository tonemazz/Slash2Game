// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UHealthBarComponent;

UCLASS()
class SLASH2_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	/* <AActor> */
	AEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	/* </AActor> */

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, const AWeapon* Weapon) override;
	virtual void Die_Implementation() override;
	/* </IHitInterace> */

protected:
	/* <BaseCharacter> */
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	virtual int32 PlayAttackMontage() override;
	virtual void Attack() override;
	/* </BaseCharacter> s*/// stufdff

	UPROPERTY() class AAIController* EnemyController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:

	/* <AI Behavior> */
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void LoseInterest();
	void StartAttackTimer();
	void ClearPatrolTimer();
	void ClearAttackTimer();	
	void PatrolTimerFinished();
	void StartPatrolling();
	void StartChasing();	
	void SetDead();
	void HideHealthBar();
	void ShowHealthBar();
	void MoveToTarget(AActor* Target);
	void PlayHitReactMontage(const FName& SectionName);
	UFUNCTION() void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent
	bool IsDead();
	bool IsAttacking();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsOutsideAttackRadius();
	bool IsOutsideCombatRadius();
	bool InTargetRange(AActor* Target, double Radius);
	FORCEINLINE bool IsEngaged() { return EnemyState == EEnemyState::EES_Engaged; }
	AActor* ChoosePatrolTarget();

	UPROPERTY(EditAnywhere) class UPawnSensingComponent* PawnSensing;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") TArray<AActor*> PatrolPoints;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation", BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) AActor* PatrolTarget;
	UPROPERTY(EditAnywhere) AActor* CombatTarget;
	UPROPERTY(EditAnywhere, Category = Combat) float AttackDelayMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat) float AttackDelayMax = 1.0f;
	UPROPERTY(EditAnywhere, Category = Combat) float PatrollingSpeed = 90.f;
	UPROPERTY(EditAnywhere, Category = Combat) float ChasingSpeed = 350.f;
	UPROPERTY(EditAnywhere) float PatrolWaitTimeMin = 5.f;
	UPROPERTY(EditAnywhere) float PatrolWaitTimeMax = 10.f;
	UPROPERTY(EditAnywhere, Category = Combat) double CombatRadius = 500.0;
	UPROPERTY(EditAnywhere, Category = Combat) double AttackRadius = 125.f;
	UPROPERTY(EditAnywhere) double PatrolRadius = 200.f;
	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;
	/* </AI Behavior> */
	
	UPROPERTY(EditAnywhere) TSubclassOf<class AWeapon> WeaponClass;
	UPROPERTY(VisibleAnywhere) UHealthBarComponent* HealthBarComponent;

};
