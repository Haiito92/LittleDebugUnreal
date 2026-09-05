// Copyright Epic Games, Inc. All Rights Reserved.

#include "LittleDebug.h"

#include "DebugCheatManagerExtension.h"
#include "LittleDebugLibrary.h"
#include "GameFramework/CheatManager.h"

#define LOCTEXT_NAMESPACE "FLittleDebugModule"

void FLittleDebugModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UCheatManager::RegisterForOnCheatManagerCreated(
		FOnCheatManagerCreated::FDelegate::CreateLambda([](UCheatManager* CheatManager)
		{
			CheatManager->AddCheatManagerExtension(NewObject<UDebugCheatManagerExtension>(CheatManager));
		}));
	
	ULittleDebugLibrary::LoadTogglesFromDisk();
}

void FLittleDebugModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLittleDebugModule, LittleDebug)