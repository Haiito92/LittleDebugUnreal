// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugCheatManagerExtension.h"

#include "LittleDebugLibrary.h"

void UDebugCheatManagerExtension::ToggleSystemDebug(const FGameplayTag& SystemTag, bool Enabled)
{
	ULittleDebugLibrary::ToggleSystemDebug(SystemTag, Enabled);
}
