// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    MinExtent = FVector(0, -2000, 0);
    MaxExtent = FVector(4000, 2000, 0);
    NavigationBoundsOffset = FVector(2000, 0, 0);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    Pool->Return(NavMeshBoundVolume);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::SetPool(UActorPool* InPool)
{
    Pool = InPool;
    PositionNavMeshBoundVolume();
}

void ATile::PositionNavMeshBoundVolume()
{
    NavMeshBoundVolume = Pool->Checkout();
    if (NavMeshBoundVolume == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool"), *GetName())
        return;
        
    }
    UE_LOG(LogTemp, Error, TEXT("[%s] Checkout: %s"), *GetName(), *NavMeshBoundVolume->GetName())

    NavMeshBoundVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
    
    GetWorld()->GetNavigationSystem()->Build();
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
    RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
/*
    TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);

    for (FSpawnPosition SpawnPosition : SpawnPositions)
    {
        PlaceActor(ToSpawn, SpawnPosition);
    }
*/
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
    RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius);
    /*
    TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, 1, 1);
    for (FSpawnPosition SpawnPosition : SpawnPositions)
    {
        PlacePawn(ToSpawn, SpawnPosition);
    }
     */
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
    int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
    
    for (int i = 0; i < NumberToSpawn; i++)
    {
        FSpawnPosition SpawnPosition;
        SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
        if (FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale))
        {
            SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
            PlaceActor(ToSpawn, SpawnPosition);
        }
    }
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
    AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
    if (Spawned)
    {
        Spawned->SetActorRelativeLocation(SpawnPosition.Location);
        Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
        Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
        Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
    }
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition& SpawnPosition)
{
    APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
    if (Spawned)
    {
        Spawned->SetActorRelativeLocation(SpawnPosition.Location);
        Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
        Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
        Spawned->SpawnDefaultController();
        Spawned->Tags.Add(FName("Enemy"));
    }
}


bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
    FBox Bound = FBox(MinExtent, MaxExtent);
    const int MAX_ATTEMPTS = 100;
    
    for (size_t i = 0; i < MAX_ATTEMPTS; i++)
    {
        FVector CandidatePoint = FMath::RandPointInBox(Bound);
        if (CanSpawnAtLocation(CandidatePoint, Radius))
        {
            OutLocation = CandidatePoint;
            return true;
        }
    }
    return false;
}



bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
    FHitResult HitResult;
    
    FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
    
    bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));
/*
    FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
    DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);
*/
    return !HasHit;
}
