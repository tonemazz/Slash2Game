// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "SlashCharacter.generated.h"



class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UGroomComponent;
class AItem;
class UAnimMontage;




UCLASS()
class SLASH2_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Sets default values for this character's properties
	ASlashCharacter();




protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input) UInputMappingContext* SlashCharacterContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input) UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input) UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input) UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input) UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input) UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input) UInputAction* EKeyAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input) UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input) FVector MovementJoystickScale;

	/* Input Callbacks */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
    void EKeyPressed(const FInputActionValue& Value);
	void Dodge();
	virtual void Attack() override;
	void EquipUnequip();
	virtual void GetHit_Implementation(const FVector& ImpactPoint, const AWeapon* Weapon) override;
	virtual void Die_Implementation() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual int32 PlayAttackMontage() override;

	/* Character States */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) ECharacterState EquipState = ECharacterState::ECS_Unequipped;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) EActionState ActionState = EActionState::EAS_Unoccupied;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) EEquipmentState EquipmentState = EEquipmentState::EAS_Unarmed;

	/* Play Montage Functions */
	void PlayEquipUnequipMontage();

	virtual bool CanAttack() override;
	bool CanMove();
	UFUNCTION(BlueprintCallable)
	void Disarm();
	UFUNCTION(BlueprintCallable)
	void Arm();



private:	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;





	/*
	Animation Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipUnequipMontage;


public:

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE AItem* GetOverlappingItem() const { return OverlappingItem; }
	FORCEINLINE void SetEquipState(ECharacterState DesiredState) { EquipState = DesiredState; }
	FORCEINLINE ECharacterState GetEquipState() const { return EquipState; }

};
