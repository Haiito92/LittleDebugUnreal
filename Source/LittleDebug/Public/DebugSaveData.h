// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugSaveData.generated.h"

/**
 * 
 */
USTRUCT()
struct LITTLEDEBUG_API FDebugSaveData
{
	GENERATED_BODY()
	
public:
	FDebugSaveData() = default;
	~FDebugSaveData() = default;
	
	UPROPERTY()
	TMap<FString, bool> Toggles;
};
