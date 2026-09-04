// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "DebugCheatManagerExtension.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class LITTLEDEBUG_API UDebugCheatManagerExtension : public UCheatManagerExtension
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Exec, BlueprintCallable)
	void ToggleSystemDebug(const FGameplayTag& SystemTag, bool Enabled);
};
