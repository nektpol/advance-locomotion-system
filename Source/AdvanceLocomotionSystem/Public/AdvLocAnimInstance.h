#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Data/AdvLocLocomotionDirectionSettings.h"
#include "Types/AdvLocLocomotionDirection.h"
#include "AdvLocAnimInstance.generated.h"

class UAdvLocAnimationControlComponent;
/**
 * 
 */
UCLASS()
class ADVANCELOCOMOTIONSYSTEM_API UAdvLocAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	void SetAnimationControlComponent(UAdvLocAnimationControlComponent* InAnimationControlComponent) {AnimationControlComponent = InAnimationControlComponent;}

protected:
	UFUNCTION(BlueprintCallable)
	UAdvLocAnimationControlComponent* GetAnimationControlComponent() const {return AnimationControlComponent;}
	
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe = true))
	EAdvLocLocomotionDirection CalculateLocomotionDirection(
		EAdvLocLocomotionDirection CurrentLocomotionDirection,
		const float CurrentLocomotionAngle,
		const FAdvLocLocomotionDirectionSettings& Settings
		);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe = true))
	bool GetHasCharacterMovement() const {return bHasCharacterMovement;}

private:
	UPROPERTY()
	TObjectPtr<UAdvLocAnimationControlComponent> AnimationControlComponent;

	bool bHasCharacterMovement;
	
};
