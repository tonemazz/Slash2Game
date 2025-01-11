// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"
#include "Slash2/DebugMacros.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::EquipWeapon(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	SetEquipped();
	Equipper = InParent->GetOwner();

	if (EquipSound) {
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());\
			UE_LOG(LogTemp, Warning, TEXT("Equipped Weapon & Played Sound"));
	}

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::SetAttackCollisionState(ECollisionEnabled::Type CollisionEnabled)
{
	if (WeaponBox) {
		WeaponBox->SetCollisionEnabled(CollisionEnabled);
	}
}






void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetInstigator());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,									// World context object (used to find the world context in which the function is called)
		Start,									// Start point of trace
		End,									// End point of trace
		FVector(5.f, 5.f, 5.f),					// Half size dimensions of the box
		BoxTraceStart->GetComponentRotation(),	// Rotation of box
		ETraceTypeQuery::TraceTypeQuery1,		// Trace channel
		false,									// Trace complex if true
		ActorsToIgnore,							// Actors to ignore in trace
		EDrawDebugTrace::None,			// Whether to draw debug trace
		BoxHit,									// Reference to the hit result
		true,									// Ignore self if true
		FLinearColor::Red,						// Trace color
		FLinearColor::Green,					// Hit color
		5.f										// Draw duration for debug trace
		);


	if (BoxHit.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());

		if (HitInterface)
		{
			PRINTLOG("Executing GetHit on Actor")
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, this);
		}

		IgnoreActors.AddUnique(BoxHit.GetActor());
		CreateFields(BoxHit.ImpactPoint);

		PRINTLOG("Applying Damage to Actor")
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(), 
			Damage, 
			GetInstigator()->GetController(), 
			this, UDamageType::StaticClass()
		);
	}
}

