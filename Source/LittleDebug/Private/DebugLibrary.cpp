// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugLibrary.h"
#include "Engine/Engine.h"
#include "Logging/StructuredLog.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"        
#include "Misc/FileHelper.h"   
#include "DebugSaveData.h"
#include "GameplayTagContainer.h"
#include "JsonObjectConverter.h"
#include "MessageType.h"
#include "Kismet/KismetSystemLibrary.h"

void UDebugLibrary::AddOnScreenDebugMessage(const FGameplayTag& SystemTag, const EDebugMessageType& MessageType, 
	const FString& Message, const FLinearColor& Color, float TimeToDisplay)
{
	if (!IsSystemDebugToggled(SystemTag)) return;
	
	InternalAddOnScreenDebugMessage(FormatMessage(SystemTag.ToString(), MessageType, Message), Color, TimeToDisplay);
}

void UDebugLibrary::Log(const FGameplayTag& SystemTag, const EDebugMessageType& MessageType, const FString& Message)
{
	if (!IsSystemDebugToggled(SystemTag)) return;
	
	FString FinalMessage = FormatMessage(SystemTag.ToString(), MessageType,  Message);
	
	InternalLog(FinalMessage, MessageType);
}

void UDebugLibrary::LogAndAddOnScreenDebugMessage(const FGameplayTag& SystemTag, const EDebugMessageType& MessageType,
	const FString& Message, const FLinearColor& Color, float TimeToDisplay)
{
	if (!IsSystemDebugToggled(SystemTag)) return;
	
	FString FinalMessage = FormatMessage(SystemTag.ToString(), MessageType,  Message);

	InternalAddOnScreenDebugMessage(FinalMessage, Color, TimeToDisplay);

	InternalLog(FinalMessage, MessageType);
}

void UDebugLibrary::DrawBox(const UObject* WorldContextObject, const FGameplayTag& SystemTag, const FVector& Center,
	const FVector& Extent, const FRotator& Rotation, const FLinearColor& LineColor, float Duration, float Thickness,
	const EDrawDebugSceneDepthPriorityGroup& DepthPriority)
{
	if (!IsSystemDebugToggled(SystemTag)) return;
	
	UKismetSystemLibrary::DrawDebugBox(WorldContextObject, Center, Extent, LineColor, Rotation, Duration, Thickness, DepthPriority);
}

void UDebugLibrary::DrawCapsule(const UObject* WorldContextObject, const FGameplayTag& SystemTag, const FVector& Center,
	float HalfHeight, float Radius, const FRotator& Rotation, const FLinearColor& LineColor, float Duration, float Thickness,
	const EDrawDebugSceneDepthPriorityGroup& DepthPriority)
{
	if (!IsSystemDebugToggled(SystemTag)) return;
	
	UKismetSystemLibrary::DrawDebugCapsule(WorldContextObject, Center, HalfHeight, Radius, Rotation, LineColor, Duration, Thickness, DepthPriority);
}

void UDebugLibrary::DrawLine(const UObject* WorldContextObject, const FGameplayTag& SystemTag, const FVector& LineStart,
	const FVector& LineEnd, const FLinearColor& LineColor, float Duration, float Thickness,
	const EDrawDebugSceneDepthPriorityGroup& DepthPriority)
{
	if (!IsSystemDebugToggled(SystemTag)) return;
	
	UKismetSystemLibrary::DrawDebugLine(WorldContextObject, LineStart, LineEnd, LineColor, Duration, Thickness, DepthPriority);
}

void UDebugLibrary::DrawSphere(const UObject* WorldContextObject, const FGameplayTag& SystemTag, const FVector& Center,
	float Radius, int32 Segments, const FLinearColor& LineColor, float Duration, float Thickness,
	const EDrawDebugSceneDepthPriorityGroup& DepthPriority)
{
	if (!IsSystemDebugToggled(SystemTag)) return;
	
	UKismetSystemLibrary::DrawDebugSphere(WorldContextObject, Center, Radius, Segments, LineColor, Duration, Thickness, DepthPriority);
}

void UDebugLibrary::ToggleSystemDebug(const FGameplayTag& SystemTag, bool Enabled)
{
	if (bool* FoundEnabled = Toggles.Find(SystemTag))
	{
		*FoundEnabled = Enabled;
		
		return;
	}
	
	Toggles.Add(SystemTag, Enabled);
	
	SaveTogglesToDisk();
}

void UDebugLibrary::InternalAddOnScreenDebugMessage(const FString& Message, const FLinearColor& Color,
	float TimeToDisplay)
{
	
	const FLinearColor ResolvedColor = (Color.A <= 0.f) ? FLinearColor::White : Color;
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		TimeToDisplay,
		ResolvedColor.ToFColor(false),
		Message
		);
}

void UDebugLibrary::InternalLog(const FString& Message, const EDebugMessageType& MessageType)
{
	switch (MessageType)
	{
	case EDebugMessageType::Log:
		{
			UE_LOGFMT(LogTemp, Log, "{0}", Message);
			break;
		}	
	case EDebugMessageType::Warning:
		{
			UE_LOGFMT(LogTemp, Warning, "{0}", Message);
			break;
		}	
	case EDebugMessageType::Error:
		{
			UE_LOGFMT(LogTemp, Error, "{0}", Message);
			break;
		}
	}
}

bool UDebugLibrary::IsSystemDebugToggled(const FGameplayTag& SystemTag)
{
	const bool* FoundEnabled = Toggles.Find(SystemTag);
	
	return FoundEnabled ? *FoundEnabled : false;
}

FString UDebugLibrary::FormatMessage(const FString& Tag, const EDebugMessageType& MessageType, const FString& Message)
{
	TArray<FStringFormatArg> Args;
	Args.Add(FStringFormatArg(Tag));
	
	switch (MessageType)
	{
	case EDebugMessageType::Log:
		{
			Args.Add(FStringFormatArg(TEXT("Log")));
			break;
		}
	case EDebugMessageType::Warning:
		{
			Args.Add(FStringFormatArg(TEXT("Warning")));
			break;
		}
	case EDebugMessageType::Error:
		{
			Args.Add(FStringFormatArg(TEXT("Error")));
			break;
		}
	}
	
	Args.Add(FStringFormatArg(Message));
	
	return FString::Format(TEXT("[{0}][{1}]: {2}"), Args);
}

void UDebugLibrary::SaveTogglesToDisk()
{
	FDebugSaveData Data;
	for (const TTuple<FGameplayTag, bool>& Pair : Toggles)
	{
		Data.Toggles.Add(Pair.Key.ToString(), Pair.Value);
	}
	
	FString OutputString;
	FJsonObjectConverter::UStructToJsonObjectString(Data, OutputString);
	
	const FString SavePath = FPaths::ProjectSavedDir() / TEXT("LittleDebug/DebugToggles.json");
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(SavePath), true);
	FFileHelper::SaveStringToFile(OutputString, *SavePath);
}

void UDebugLibrary::LoadTogglesFromDisk()
{
	const FString SavePath = FPaths::ProjectSavedDir() / TEXT("LittleDebug/DebugToggles.json");

	FString FileContent;
	if (!FFileHelper::LoadFileToString(FileContent, *SavePath))
	{
		return;
	}
	
	FDebugSaveData Data;
	if (!FJsonObjectConverter::JsonObjectStringToUStruct(FileContent, &Data, 0, 0))
	{
		return;
	}
	
	Toggles.Empty();
	for (const TPair<FString, bool>& Pair : Data.Toggles)
	{
		const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*Pair.Key), false);
		if (Tag.IsValid())
		{
			Toggles.Add(Tag, Pair.Value);
		}
	}
}

TMap<FGameplayTag, bool> UDebugLibrary::Toggles = {};
