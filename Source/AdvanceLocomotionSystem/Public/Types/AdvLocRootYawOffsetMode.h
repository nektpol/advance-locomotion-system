#pragma once

#include "CoreMinimal.h"
#include "AdvLocRootYawOffsetMode.generated.h"

UENUM(BlueprintType)
enum class EAdvLocRootYawOffsetMode : uint8
{
	Accumulate,
	BlendOut,
	Hold,
};