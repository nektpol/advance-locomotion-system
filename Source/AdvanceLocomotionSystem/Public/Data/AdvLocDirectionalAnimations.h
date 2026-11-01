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


USTRUCT(BlueprintType)
struct ADVANCELOCOMOTIONSYSTEM_API FAdvLocTurnInPlaceAnimations
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> TurnInPlaceLeft180Animation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> TurnInPlaceLeft90Animation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> TurnInPlaceRight180Animation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> TurnInPlaceRight90Animation;
};

USTRUCT(BlueprintType)
struct ADVANCELOCOMOTIONSYSTEM_API FAdvLocTurnInPlaceAnimationsPack
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAdvLocTurnInPlaceAnimations StandTurnInPlaceAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAdvLocTurnInPlaceAnimations CrouchTurnInPlaceAnimations;
};