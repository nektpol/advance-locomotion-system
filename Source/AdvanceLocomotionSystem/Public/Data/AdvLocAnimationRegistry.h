// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Data/AdvLocResolvedAnimationProfile.h"
#include "AdvLocAnimationRegistry.generated.h"

class UAdvLocAnimationProfile;

UCLASS(BlueprintType)
class ADVANCELOCOMOTIONSYSTEM_API UAdvLocAnimationRegistry : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Registry")
	TObjectPtr<UAdvLocAnimationRegistry> ParentRegistry;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Registry")
	TArray<TObjectPtr<UAdvLocAnimationProfile>> Profiles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Registry")
	FGameplayTag DefaultProfileTag;

	UFUNCTION(BlueprintPure, Category = "Animation|Registry")
	UAdvLocAnimationProfile* FindProfileExact(FGameplayTag ProfileTag) const;

	UFUNCTION(BlueprintPure, Category = "Animation|Registry")
	FGameplayTag GetEffectiveDefaultProfileTag() const;
	
	UFUNCTION(BlueprintCallable, Category = "Animation|Registry")
	bool ResolveProfile(FGameplayTag RequestedProfileTag, FAdvLocResolvedAnimationProfile& OutResolvedProfile) const;
};
