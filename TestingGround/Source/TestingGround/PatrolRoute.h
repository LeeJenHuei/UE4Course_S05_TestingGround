

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolRoute.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUND_API UPatrolRoute : public UActorComponent
{
	GENERATED_BODY()

public:
    TArray<AActor*> GetPatrolPoints() const;
    
private:
    UPROPERTY(EditInstanceOnly, Category = "Patrol Route")
    TArray<AActor*> PatrolPoints = TArray<AActor*>();
	
};
