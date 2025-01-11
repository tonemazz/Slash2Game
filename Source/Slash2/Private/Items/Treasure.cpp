// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);

	if (SlashCharacter) {
		SlashCharacter->SetOverlappingItem(this);
		Destroy();
		SlashCharacter->GetAttributes()->AddGold(GoldValue);
		int32 GoldAmt = SlashCharacter->GetAttributes()->GetGold();
		UE_LOG(LogTemp, Warning, TEXT("Gold: %d"), GoldAmt);

		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}

	}
}
