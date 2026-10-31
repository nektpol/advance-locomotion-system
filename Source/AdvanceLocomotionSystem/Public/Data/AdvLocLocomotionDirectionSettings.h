#pragma once

#include "CoreMinimal.h"
#include "AdvLocLocomotionDirectionSettings.generated.h"

USTRUCT(BlueprintType)
struct ADVANCELOCOMOTIONSYSTEM_API FAdvLocLocomotionDirectionSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BackwardMin = -130.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BackwardMax = 130.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForwardMin = -50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForwardMax = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeadZone = 20.f;
};
