
#include "Data/AdvLocAnimationRegistry.h"
#include "Data/AdvLocAnimationProfile.h"

UAdvLocAnimationProfile* UAdvLocAnimationRegistry::FindProfileExact(FGameplayTag ProfileTag) const
{
    if (!ProfileTag.IsValid()) return nullptr;

    TSet<const UAdvLocAnimationRegistry*> VisitedRegistries;
    const UAdvLocAnimationRegistry* CurrentRegistry = this;

    while (IsValid(CurrentRegistry))
    {
        if (VisitedRegistries.Contains(CurrentRegistry))
        {
            UE_LOG(LogTemp, Error, TEXT("AdvLoc: Circular registry reference detected."));
            return nullptr;
        }

        VisitedRegistries.Add(CurrentRegistry);

        UAdvLocAnimationProfile* FoundProfile = nullptr;

        for (const TObjectPtr<UAdvLocAnimationProfile>& Profile : CurrentRegistry->Profiles)
        {
            if (!IsValid(Profile.Get())) continue;
            if (Profile->ProfileTag != ProfileTag) continue;

            if (FoundProfile)
            {
                UE_LOG(LogTemp, Error, TEXT("AdvLoc: Duplicate profile tag %s in registry %s."), *ProfileTag.ToString(), *CurrentRegistry->GetName());
                return nullptr;
            }

            FoundProfile = Profile.Get();
        }

        if (FoundProfile) return FoundProfile;

        CurrentRegistry = CurrentRegistry->ParentRegistry.Get();
    }

    return nullptr;
}

FGameplayTag UAdvLocAnimationRegistry::GetEffectiveDefaultProfileTag() const
{
    TSet<const UAdvLocAnimationRegistry*> VisitedRegistries;
    const UAdvLocAnimationRegistry* CurrentRegistry = this;

    while (IsValid(CurrentRegistry))
    {
        if (VisitedRegistries.Contains(CurrentRegistry))
        {
            UE_LOG(LogTemp, Error, TEXT("AdvLoc: Circular registry reference detected."));
            return FGameplayTag();
        }

        VisitedRegistries.Add(CurrentRegistry);

        if (CurrentRegistry->DefaultProfileTag.IsValid()) return CurrentRegistry->DefaultProfileTag;

        CurrentRegistry = CurrentRegistry->ParentRegistry.Get();
    }

    return FGameplayTag();
}

bool UAdvLocAnimationRegistry::ResolveProfile(FGameplayTag RequestedProfileTag, FAdvLocResolvedAnimationProfile& OutResolvedProfile) const
{
    OutResolvedProfile = FAdvLocResolvedAnimationProfile();

    FGameplayTag SelectedProfileTag = RequestedProfileTag;

    if (!SelectedProfileTag.IsValid() || !IsValid(FindProfileExact(SelectedProfileTag)))
    {
        SelectedProfileTag = GetEffectiveDefaultProfileTag();

        if (!SelectedProfileTag.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("AdvLoc: Cannot resolve profile. No valid default profile tag."));
            return false;
        }

        if (RequestedProfileTag.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("AdvLoc: Profile %s not found. Trying default profile %s."), *RequestedProfileTag.ToString(), *SelectedProfileTag.ToString());
        }
    }

    TSet<FGameplayTag> VisitedTags;
    TArray<const UAdvLocAnimationProfile*> ProfileChain;

    FGameplayTag CurrentTag = SelectedProfileTag;

    while (CurrentTag.IsValid())
    {
        if (VisitedTags.Contains(CurrentTag))
        {
            UE_LOG(LogTemp, Error, TEXT("AdvLoc: Circular profile fallback detected at %s."), *CurrentTag.ToString());
            return false;
        }

        VisitedTags.Add(CurrentTag);

        const UAdvLocAnimationProfile* CurrentProfile = FindProfileExact(CurrentTag);

        if (!IsValid(CurrentProfile))
        {
            UE_LOG(LogTemp, Error, TEXT("AdvLoc: Profile %s is missing from the fallback chain."), *CurrentTag.ToString());
            return false;
        }

        ProfileChain.Add(CurrentProfile);
        CurrentTag = CurrentProfile->FallbackProfileTag;
    }

    OutResolvedProfile.RequestedProfileTag = RequestedProfileTag;
    OutResolvedProfile.SelectedProfileTag = SelectedProfileTag;

    bool bAllowUpperBodyInheritance = true;

    for (const UAdvLocAnimationProfile* Profile : ProfileChain)
    {
        OutResolvedProfile.ResolutionPath.Add(Profile->ProfileTag);

        if (OutResolvedProfile.FullBodyLayerClass.IsNull() && !Profile->FullBodyLayerClass.IsNull())
        {
            OutResolvedProfile.FullBodyLayerClass = Profile->FullBodyLayerClass;
            OutResolvedProfile.FullBodySourceTag = Profile->ProfileTag;
        }

        if (bAllowUpperBodyInheritance && OutResolvedProfile.UpperBodyLayerClass.IsNull() && !Profile->UpperBodyLayerClass.IsNull())
        {
            OutResolvedProfile.UpperBodyLayerClass = Profile->UpperBodyLayerClass;
            OutResolvedProfile.UpperBodySourceTag = Profile->ProfileTag;
        }

        if (!Profile->bInheritUpperBodyLayer)
        {
            bAllowUpperBodyInheritance = false;
        }
    }

    return true;
}

