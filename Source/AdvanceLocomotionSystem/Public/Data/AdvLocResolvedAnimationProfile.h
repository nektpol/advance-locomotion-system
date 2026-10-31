#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "AdvLocResolvedAnimationProfile.generated.h"

USTRUCT(BlueprintType)
struct ADVANCELOCOMOTIONSYSTEM_API FAdvLocResolvedAnimationProfile
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Profile")
	FGameplayTag RequestedProfileTag;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Profile")
	FGameplayTag SelectedProfileTag;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Profile")
	TArray<FGameplayTag> ResolutionPath;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Layers")
	TSoftClassPtr<UAnimInstance> FullBodyLayerClass;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Layers")
	FGameplayTag FullBodySourceTag;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Layers")
	TSoftClassPtr<UAnimInstance> UpperBodyLayerClass;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Layers")
	FGameplayTag UpperBodySourceTag;
};