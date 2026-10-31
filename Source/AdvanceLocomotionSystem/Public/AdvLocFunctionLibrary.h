#pragma once

#include "CoreMinimal.h"
#include "Data/AdvLocDirectionalAnimations.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/AdvLocLocomotionDirection.h"
#include "Types/AdvLocTypes.h"
#include "AdvLocFunctionLibrary.generated.h"


UCLASS(meta=(BlueprintThreadSafe))
class ADVANCELOCOMOTIONSYSTEM_API UAdvLocFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "AdvanceLocomotionSystem")
	static UAnimSequence* GetDirectionalAnimation(
		const FAdvLocDirectionalAnimationsPack& DirectionalAnimationsPack,
		const EAdvLocLocomotionDirection Direction, 
		const EAdvLocGait Gait, 
		const EAdvLocStance Stance);
};
