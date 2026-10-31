#pragma once

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "AdvLocAnimationProfile.generated.h"

UCLASS(BlueprintType)
class ADVANCELOCOMOTIONSYSTEM_API UAdvLocAnimationProfile : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation|Profile")
	FGameplayTag ProfileTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation|Profile")
	FGameplayTag FallbackProfileTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation|Layers")
	TSoftClassPtr<UAnimInstance> FullBodyLayerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation|Layers")
	TSoftClassPtr<UAnimInstance> UpperBodyLayerClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation|Layers")
	bool bInheritUpperBodyLayer = true;
};
