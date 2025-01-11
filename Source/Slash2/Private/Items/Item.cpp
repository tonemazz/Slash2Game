// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash2/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	// Binding callback function
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	

	// Random values for position and rotation
// double RandomX = FMath::RandRange(-1000.f, 1000.f);
// double RandomY = FMath::RandRange(-1000.f, 1000.f);
// double RandomZ = FMath::RandRange(-1000.f, 1000.f);
// double RandomXRot = FMath::RandRange(0.f, 360.f);
// double RandomYRot = FMath::RandRange(0.f, 360.f);
// double RandomZRot = FMath::RandRange(0.f, 360.f);
// 
// 
// // Track spawn position and set new position
// FVector OriginalPosition = GetActorLocation();
// SetActorLocation(FVector(RandomX, RandomY, RandomZ));
// FVector OffsetPosition = GetActorLocation();
// 
// //Draw line from spawn position to set position 
// DRAW_LINE(OriginalPosition, OffsetPosition);
// OriginalPosition = OffsetPosition;
// 
// // Set new rotation and add position/rotiation offset
// SetActorRotation(FRotator(RandomXRot, RandomYRot, RandomZRot));
// AddActorWorldOffset(FVector(0, 0, 1000));
// AddActorWorldRotation(FRotator(0, 0, 134));
// OffsetPosition = GetActorLocation();
// 
// // Draw line from original position to offset position
// DRAW_LINE(OriginalPosition, OffsetPosition);




// FVector Location = GetActorLocation();
// FVector ForwardVector = GetActorForwardVector();
// DRAW_BOX(Location);
// DRAW_SPHERE_CUSTOM(Location, FColor::Silver);
// DRAW_VECTOR(Location, Location + ForwardVector * 100);



}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

//Callback function for OnComponentBeginOverlap
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);


	if (SlashCharacter) {
		SlashCharacter->SetOverlappingItem(this);
	}

	
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);


	if (SlashCharacter) {
		SlashCharacter->SetOverlappingItem(nullptr);
	}
}



// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if (bRotate && !Equipped)
	{
		FRotator NewRotation = GetActorRotation();
		
		NewRotation.Roll = RotationSpeedX * DeltaTime;
		NewRotation.Pitch = RotationSpeedY * DeltaTime;
		NewRotation.Yaw = RotationSpeedZ * DeltaTime;
		AddActorWorldRotation(NewRotation);
	}

	if (!Equipped) {
		AddActorWorldOffset(FVector(0, 0, TransformedSin()));
	}



}

