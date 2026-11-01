#pragma once

#include "CoreMinimal.h"
#include "AdvLocPredictionData.generated.h"


USTRUCT(BlueprintType)
struct ADVANCELOCOMOTIONSYSTEM_API FAdvLocStopPredictionData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvanceLocomotionSystem")
	FVector Velocity = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvanceLocomotionSystem")
	bool bUseSeparateBrakingFriction = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvanceLocomotionSystem")
	float BrakingFriction = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvanceLocomotionSystem")
	float GroundFriction = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvanceLocomotionSystem")
	float BrakingFrictionFactor	= 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvanceLocomotionSystem")
	float BrakingDecelerationWalking = 0;
};

USTRUCT(BlueprintType)
struct ADVANCELOCOMOTIONSYSTEM_API FAdvLocPivotPredictionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvanceLocomotionSystem")
	FVector Acceleration = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvanceLocomotionSystem")
	FVector Velocity = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvanceLocomotionSystem")
	float GroundFriction = 0;
};