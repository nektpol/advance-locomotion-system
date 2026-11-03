#pragma once

#include "CoreMinimal.h"
#include "AdvLocTypes.generated.h"

UENUM(BlueprintType)
enum class EAdvLocGait : uint8
{
	None,
	Walk,
	Jog,
};

UENUM(BlueprintType)
enum class EAdvLocStance : uint8
{
	Undefined,
	Stand,
	Crouch
};

UENUM(BlueprintType)
enum class EAdvLocLocomotionStyle : uint8
{
	Undefined,
	Forward,
	Directional
};


