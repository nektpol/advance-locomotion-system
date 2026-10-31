#pragma once

#include "CoreMinimal.h"
#include "AdvLocLocomotionDirection.generated.h"

UENUM(BlueprintType)
enum class EAdvLocLocomotionDirection : uint8
{
	Backward,
	Forward,
	Left,
	Right,
};
