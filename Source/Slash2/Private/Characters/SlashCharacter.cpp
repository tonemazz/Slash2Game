#include "Characters/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Slash2/Public/Items/Item.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Slash2/Public/Items/Weapons/Weapon.h"
#include "Slash2/DebugMacros.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Camera setup
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 145.f;

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();


    Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	GetMesh()->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Overlap);





	// Character model will orient itself to the movement direction
	MovementComponent->bOrientRotationToMovement = true;
	// The speed at which the character orients to the movement direction
	MovementComponent->RotationRate = FRotator(0.0f, 720.0f, 0.0f);



	// Disable rotation of the character with the controller when set to false

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


}


void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("SlashCharacter"));

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(SlashCharacterContext, 0);
		}
	}
}


void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EquipUnequip);
	}
}


void ASlashCharacter::Move(const FInputActionValue& Value)
{
	
	if (CanMove()) {
		// Passing our raw input values to the function
			const FVector2D InputAxisVector = Value.Get<FVector2D>();
		
			const FRotator ControllerRotation = GetController()->GetControlRotation();
		
			// We're only concerned with the horizontal facing direction of the controller
			const FRotator ControllerHorizontalRotation(0.f, ControllerRotation.Yaw, 0.f);
		
			// We get the forward vector from the controller's rotation by doing a rotation matrix transformation on the controller's horizontal (Yaw) rotation.
			// As a result, ForwardDirection will always be the vector forward of our standard camera view, irrespective of the camera's inclination.
			const FVector ControllerForwardDirection = FRotationMatrix(ControllerHorizontalRotation).GetUnitAxis(EAxis::X); // GetUnitAxis returns a unit vector, which is desired for the Forward Vector.
			AddMovementInput(ControllerForwardDirection, InputAxisVector.Y);
			const FVector ControllerRightDirection = FRotationMatrix(ControllerHorizontalRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(ControllerRightDirection, InputAxisVector.X);
	}
	
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ASlashCharacter::Jump()
{
	Super::Jump();
}

void ASlashCharacter::EKeyPressed(const FInputActionValue& Value)
{

	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	UE_LOG(LogTemp, Warning, TEXT("Casted To Weapon"));
	{
		if (OverlappingWeapon && EquipmentState != EEquipmentState::EAS_HasWeapon )
		{
			OverlappingWeapon->EquipWeapon(GetMesh(), FName("RightHandSocket"), this, this);
			EquipState = ECharacterState::ECS_EquippedOneHanded;
			EquipmentState = EEquipmentState::EAS_HasWeapon;
			UE_LOG(LogTemp, Warning, TEXT("Equipped Weapon"));
			EquippedWeapon = OverlappingWeapon;
			OverlappingWeapon = nullptr;

		}

	}
	}




void ASlashCharacter::Dodge()
{
}

void ASlashCharacter::Attack()
{


	if (CanAttack())
	{
		ActionState = EActionState::EAS_Attacking;
		PlayAttackMontage();
	}


}

void ASlashCharacter::EquipUnequip()
{
	if (ActionState == EActionState::EAS_Unoccupied && EquipmentState == EEquipmentState::EAS_HasWeapon)
	{
		PlayEquipUnequipMontage();

		if (EquipState == ECharacterState::ECS_Unequipped && EquipmentState == EEquipmentState::EAS_HasWeapon)
		{
			EquipState = ECharacterState::ECS_EquippedOneHanded;
		}
		else if (EquipState != ECharacterState::ECS_Unequipped && EquipmentState == EEquipmentState::EAS_HasWeapon)
		{
			EquipState = ECharacterState::ECS_Unequipped;
		}

		ActionState = EActionState::EAS_Equipping;
	}
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, const AWeapon* Weapon)
{
	PlayHitSound(ImpactPoint);
	PlayHitParticles(ImpactPoint);
	if (IsAlive()) { DirectionalHitReact(ImpactPoint); }
}

void ASlashCharacter::Die_Implementation()
{
	PRINTLOG("Executing Die Function")
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes)
	{
		Attributes->RecieveDamage(DamageAmount);

		if (!Attributes->IsAlive())
		{
			Execute_Die(this);
		}

	}
	return DamageAmount;
}

int32 ASlashCharacter::PlayAttackMontage()
{
	return Super::PlayAttackMontage();
}

void ASlashCharacter::PlayEquipUnequipMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FName Choice = FName();

	if (AnimInstance && EquipUnequipMontage)
	{
		if (EquipState == ECharacterState::ECS_Unequipped) {
			Choice = FName("Equip");
		}
		else {
			Choice = FName("Unequip");
		}
		
		
		AnimInstance->Montage_Play(EquipUnequipMontage);


		AnimInstance->Montage_JumpToSection(Choice, EquipUnequipMontage);
	}
}

bool ASlashCharacter::CanAttack()
{
	return	ActionState == EActionState::EAS_Unoccupied &&
			EquipState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanMove()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void ASlashCharacter::Disarm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("BackSocket"));
	}
}

void ASlashCharacter::Arm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

