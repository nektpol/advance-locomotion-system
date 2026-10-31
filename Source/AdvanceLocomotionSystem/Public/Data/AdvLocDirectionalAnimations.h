#pragma once

#include "CoreMinimal.h"
#include "AdvLocDirectionalAnimations.generated.h"

USTRUCT(BlueprintType)
struct ADVANCELOCOMOTIONSYSTEM_API FAdvLocDirectionalAnimations
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Directional Animations")
	TObjectPtr<UAnimSequence> BackwardAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Directional Animations")
	TObjectPtr<UAnimSequence> ForwardAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Directional Animations")
	TObjectPtr<UAnimSequence> LeftAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Directional Animations")
	TObjectPtr<UAnimSequence> RightAnimation;
};

USTRUCT(BlueprintType)
struct ADVANCELOCOMOTIONSYSTEM_API FAdvLocDirectionalAnimationsPack
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAdvLocDirectionalAnimations WalkDirectionalAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAdvLocDirectionalAnimations JogDirectionalAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAdvLocDirectionalAnimations CrouchWalkDirectionalAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAdvLocDirectionalAnimations CrouchJogDirectionalAnimations;
};
