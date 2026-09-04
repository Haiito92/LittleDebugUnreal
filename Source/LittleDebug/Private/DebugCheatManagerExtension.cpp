// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugCheatManagerExtension.h"

#include "DebugLibrary.h"

void UDebugCheatManagerExtension::ToggleSystemDebug(const FGameplayTag& SystemTag, bool Enabled)
{
	UDebugLibrary::ToggleSystemDebug(SystemTag, Enabled);
}
