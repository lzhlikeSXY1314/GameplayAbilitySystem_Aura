// Copy by 2598876534qq.com


#include "Data/AttributeInfo.h"
#include <AuraGameplayTags.h>

UAttributeInfo::UAttributeInfo()
{
    const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

    AttributeInformation = {
        { GameplayTags.Attributes_Primary_Strength, FText::FromString("Strength"), FText::FromString("Increases physical damage") },
        { GameplayTags.Attributes_Primary_Intelligence, FText::FromString("Intelligence"), FText::FromString("Increases magical damage") },
        { GameplayTags.Attributes_Primary_Resilience, FText::FromString("Resilience"), FText::FromString("Increases Armor and Armor Penetration") },
        { GameplayTags.Attributes_Primary_Vigor, FText::FromString("Vigor"), FText::FromString("Increases Health") },

        { GameplayTags.Attributes_Secondary_Armor, FText::FromString("Armor"), FText::FromString("Reduces damage taken, improves Block Chance") },
        { GameplayTags.Attributes_Secondary_ArmorPenetration, FText::FromString("Armor Penetration"), FText::FromString("Ignores Percentage of enemy Armor, increases Critical Hit Chance") },
        { GameplayTags.Attributes_Secondary_BlockChance, FText::FromString("Block Chance"), FText::FromString("Chance to cut incoming damage in half") },
        { GameplayTags.Attributes_Secondary_CriticalHitChance, FText::FromString("Critical Hit Chance"), FText::FromString("Chance to double damage plus critical hit bonus") },
        { GameplayTags.Attributes_Secondary_CriticalHitDamage, FText::FromString("Critical Hit Damage"), FText::FromString("Bonus damage added when a critical hit is scored") },
        { GameplayTags.Attributes_Secondary_CriticalHitResistance, FText::FromString("Critical Hit Resistance"), FText::FromString("Reduces Critical Hit Chance of attacking enemies") },
        { GameplayTags.Attributes_Secondary_HealthRegeneration, FText::FromString("Health Regeneration"), FText::FromString("Amount of Health regenerated every 1 second") },
        { GameplayTags.Attributes_Secondary_ManaRegeneration, FText::FromString("Mana Regeneration"), FText::FromString("Amount of Mana regenerated every 1 second") },
        { GameplayTags.Attributes_Secondary_MaxHealth, FText::FromString("Max Health"), FText::FromString("Maximum amount of Health obtainable") },
        { GameplayTags.Attributes_Secondary_MaxMana, FText::FromString("Max Mana"), FText::FromString("Maximum amount of Mana obtainable") },
    };
}

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}

