// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LittleDebugLibrary.generated.h"

/**
 * 
 */

enum class EDebugMessageType : uint8;
struct FGameplayTag;

UCLASS()
class LITTLEDEBUG_API ULittleDebugLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	friend class FLittleDebugModule;
	
public:
	UFUNCTION(BlueprintCallable, Category="Little Debug", DisplayName="Print", meta=(AutoCreateRefTerm="SystemTag,MessageType,Color", DevelopmentOnly))
	static void AddOnScreenDebugMessage(const FGameplayTag& SystemTag, const EDebugMessageType& MessageType, const FString& Message, const FLinearColor& Color = FLinearColor::White, float TimeToDisplay = 0.0f);
	
	UFUNCTION(BlueprintCallable, Category="Little Debug", DisplayName="Log", meta=(AutoCreateRefTerm="SystemTag,MessageType", DevelopmentOnly))
	static void Log(const FGameplayTag& SystemTag, const EDebugMessageType& MessageType, const FString& Message);
	
	UFUNCTION(BlueprintCallable, Category="Little Debug", DisplayName="Log & Print", meta=(AutoCreateRefTerm="SystemTag,MessageType,Color", DevelopmentOnly))
	static void LogAndAddOnScreenDebugMessage(const FGameplayTag& SystemTag, const EDebugMessageType& MessageType, const FString& Message, const FLinearColor& Color = FLinearColor::White, float TimeToDisplay = 0.0f);
	
	UFUNCTION(BlueprintCallable, Category="Little Debug", meta=(WorldContext="WorldContextObject", AutoCreateRefTerm="SystemTag,Center,Extent,Rotation,LineColor,DepthPriority", DevelopmentOnly))
	static void DrawBox(const UObject* WorldContextObject, const FGameplayTag& SystemTag, const FVector& Center, const FVector& Extent, const FRotator& Rotation = FRotator::ZeroRotator, const FLinearColor& LineColor = FLinearColor::White, float Duration = 0.f, float Thickness = 0.f, const EDrawDebugSceneDepthPriorityGroup& DepthPriority = EDrawDebugSceneDepthPriorityGroup::World);
	
	UFUNCTION(BlueprintCallable, Category="Little Debug", meta=(WorldContext="WorldContextObject", AutoCreateRefTerm="SystemTag,Center,Rotation,LineColor,DepthPriority", DevelopmentOnly))
	static void DrawCapsule(const UObject* WorldContextObject, const FGameplayTag& SystemTag, const FVector& Center, float HalfHeight, float Radius, const FRotator& Rotation = FRotator::ZeroRotator, const FLinearColor& LineColor = FLinearColor::White, float Duration = 0.f, float Thickness = 0.f, const EDrawDebugSceneDepthPriorityGroup& DepthPriority = EDrawDebugSceneDepthPriorityGroup::World);
	
	UFUNCTION(BlueprintCallable, Category="Little Debug", meta=(WorldContext="WorldContextObject", AutoCreateRefTerm="SystemTag,LineStart,LineEnd,LineColor,DepthPriority", DevelopmentOnly))
	static void DrawLine(const UObject* WorldContextObject, const FGameplayTag& SystemTag, const FVector& LineStart, const FVector& LineEnd, const FLinearColor& LineColor = FLinearColor::White, float Duration = 0.f, float Thickness = 0.f, const EDrawDebugSceneDepthPriorityGroup& DepthPriority = EDrawDebugSceneDepthPriorityGroup::World);
	
	UFUNCTION(BlueprintCallable, Category="Little Debug", meta=(WorldContext="WorldContextObject", AutoCreateRefTerm="SystemTag,Center,LineColor,DepthPriority", DevelopmentOnly))
	static void DrawSphere(const UObject* WorldContextObject, const FGameplayTag& SystemTag, const FVector& Center, float Radius = 100.f, int32 Segments = 12, const FLinearColor& LineColor = FLinearColor::White, float Duration = 0.f, float Thickness = 0.f, const EDrawDebugSceneDepthPriorityGroup& DepthPriority = EDrawDebugSceneDepthPriorityGroup::World);
	
	UFUNCTION(BlueprintCallable, Category="Little Debug", meta=(DevelopmentOnly))
	static void ToggleSystemDebug(const FGameplayTag& SystemTag, bool Enabled);
private:
	
	static void InternalAddOnScreenDebugMessage(const FString& Message, const FLinearColor& Color = FLinearColor::White, float TimeToDisplay = 0.0f);
	static void InternalLog(const FString& Message, const EDebugMessageType& MessageType);
	
	UFUNCTION(meta=(DevelopmentOnly))
	static bool IsSystemDebugToggled(const FGameplayTag& SystemTag);
	
	UFUNCTION(meta=(DevelopmentOnly))
	static FString FormatMessage(const FString& Tag, const EDebugMessageType& MessageType, const FString& Message);
	
	static void SaveTogglesToDisk();
	static void LoadTogglesFromDisk();
	
	static TMap<FGameplayTag, bool> Toggles;
};
