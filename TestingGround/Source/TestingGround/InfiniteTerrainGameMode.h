// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestingGroundGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AInfiniteTerrainGameMode : public ATestingGroundGameMode
{
	GENERATED_BODY()
	
public:
    AInfiniteTerrainGameMode();
    
    UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
    void PopulateBoundsVolumePool();
    
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
    class UActorPool* NavMeshBoundsVolumePool;
    
private:
    void AddToPool(class ANavMeshBoundsVolume* VolumeToAdd);
	
};
