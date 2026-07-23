// Copy by 2598876534qq.com

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextDamage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageTextDamage : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetDamageText(float Damage);

};
