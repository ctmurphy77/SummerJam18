// Fill out your copyright notice in the Description page of Project Settings.

#include "TileActor.h"
#include "GameFramework/Actor.h"
#include "WorldCollision.h"
#include "DrawDebugHelpers.h"
#include "TwinStickShooter.h"

// Sets default values
ATileActor::ATileActor()
{

}

// Called when the game starts or when spawned
void ATileActor::BeginPlay()
{
	Super::BeginPlay();

	CanSpawnAtLocation(GetActorLocation(), 30.0f);
}

TArray<AActor*> ATileActor::PlaceActors(TSubclassOf<AActor> ToSpawn, int NumToSpawn, float ItemRadius)
{
	TArray<AActor*> Actors;
	for (size_t i = 0; i < NumToSpawn; i++) {
		FVector SpawnPoint;
		if (FindEmptyLocation(SpawnPoint, ItemRadius)) {
			float RandomRotation = FMath::RandRange(-180.0f, 180.0f);
			Actors.Add(PlaceActor(ToSpawn, SpawnPoint, RandomRotation));
		}
	}

	return Actors;
}

bool ATileActor::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FVector Min(-3000, -3000, 0);
	FVector Max(3000, 3000, 0);

	FBox Bounds(Min, Max);

	const int MAX_ATTEMPTS = 50;

	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius)) {
			OutLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}

AActor* ATileActor::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::KeepRelative, false);
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPoint);
	Spawned->AttachToActor(this, TransformRules);
	Spawned->SetActorRotation(FRotator(0, Rotation, 0));
	return Spawned;
}


bool ATileActor::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation, FQuat::Identity, COLLISION_SPAWN, FCollisionShape::MakeSphere(Radius));
	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);

	return !HasHit;
}
