#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Get the owner of this animation instance - i.e. the character who this instance will effect on.
	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());

	if (SlashCharacter)
	{
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}


}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (SlashCharacterMovement)
	{
        GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);
		isFalling = SlashCharacterMovement->IsFalling();
		CharacterState = SlashCharacter->GetEquipState();
		
	}
}
