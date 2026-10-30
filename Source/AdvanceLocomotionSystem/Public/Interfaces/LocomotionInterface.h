#pragma once

#include "CoreMinimal.h"
#include "AnimationControlComponent.h"
#include "Enums/WeaponType.h"
#include "UObject/Interface.h"
#include "LocomotionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULocomotionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCELOCOMOTIONSYSTEM_API ILocomotionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable ,Category = "Locomotion")
	void SetWeaponType(EWeaponType NewWeaponType);

};
