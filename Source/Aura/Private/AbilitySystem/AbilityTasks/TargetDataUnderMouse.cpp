// Copy by 2598876534qq.com


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include"AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
    UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
    return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
    const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
    if (bIsLocallyControlled)
    {
        SendMouseCursorData();
    }
    else
    {
        //TODO: We are on the server, so listen for target data.
        const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
        const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
        AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this,&ThisClass::OnTargetDataReplicatedCallback);
        const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,ActivationPredictionKey);
        if (!bCalledDelegate)
        {
            SetWaitingOnRemotePlayerData();
        }



    }


}

void UTargetDataUnderMouse::SendMouseCursorData()
{
    FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get()); //开启 GAS 预测窗口，让客户端能安全地执行预测逻辑（比如本地先生成特效），同时保证服务器最终权威校验。
    
    APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
    FHitResult CursorHit;
    PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit); //从本地 PlayerController 执行射线检测，拿到鼠标下的世界位置与碰撞信息。

    FGameplayAbilityTargetDataHandle DataHandle;
    FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
    Data->HitResult = CursorHit;
    DataHandle.Add(Data); //将射线结果封装成 GAS 标准的 FGameplayAbilityTargetDataHandle，用于网络传输和后续技能逻辑。

    AbilitySystemComponent->ServerSetReplicatedTargetData(
        GetAbilitySpecHandle(),
        GetActivationPredictionKey(),
        DataHandle,
        FGameplayTag(),
        AbilitySystemComponent->ScopedPredictionKey
    ); //通过 RPC 将目标数据发送到服务器，服务器会验证数据合法性并执行核心技能逻辑（如生成 projectile、施加伤害）。

    if (ShouldBroadcastAbilityTaskDelegates())
    {
        ValidData.Broadcast(DataHandle);
    } //在客户端本地触发委托，用于立即执行本地表现逻辑（如播放技能音效、生成粒子特效），实现 “预测” 效果，提升手感。

}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
    AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        ValidData.Broadcast(DataHandle);
    }
}
