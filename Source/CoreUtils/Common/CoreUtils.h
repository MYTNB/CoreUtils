// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
namespace CoreUtils
{
#pragma region SAFE GET
	FORCEINLINE static AActor* SafeGetOwner(const UActorComponent* InComponent) { return InComponent ? InComponent->GetOwner() : nullptr; }
	FORCEINLINE static UClass* SafeGetClass(const UObject* InObject) { return InObject ? InObject->GetClass() : nullptr; }
#pragma endregion
	
#pragma region Log
	template <class TClass>
	FORCEINLINE static FName ToLogFName() { return TClass::StaticClass()->GetFName(); }
	FORCEINLINE static FString ToLogString(UObject* InObject) { return GetNameSafe(InObject); }
	FORCEINLINE static FString ToLogString(const UObject* InObject) { return GetNameSafe(InObject); }
	FORCEINLINE static FString ToLogString(UClass* InClass) { return GetNameSafe(InClass); }
	FORCEINLINE static FString ToLogString(const UClass* InClass) { return GetNameSafe(InClass); }
	FORCEINLINE static FString ToLogString(AActor* InActor) { return GetNameSafe(InActor); }
	FORCEINLINE static FString ToLogString(const AActor* InActor) { return GetNameSafe(InActor); }
	FORCEINLINE static FString ToLogString(UActorComponent* InComponent) { return FString::Printf(TEXT("%s.%s"), *ToLogString(SafeGetOwner(InComponent)), *ToLogString(SafeGetClass(InComponent))); }
	FORCEINLINE static FString ToLogString(const UActorComponent* InComponent) { return FString::Printf(TEXT("%s.%s"), *ToLogString(SafeGetOwner(InComponent)), *ToLogString(SafeGetClass(InComponent))); }

	inline FString GetObjectNameWithOuter(const UObject* Object)
	{
		if (IsValid(Object))
		{
			UObject* StopOuter = nullptr;
			const auto& Outer1 = Object->GetOuter();
			if (IsValid(Outer1))
			{
				StopOuter = Outer1;
				const auto& Outer2 = Outer1->GetOuter();
				if (IsValid(Outer2))
				{
					StopOuter = Outer2;
				}
			}

			if (IsValid(StopOuter))
			{
				return FString::Printf(TEXT("%s"), *Object->GetPathName(StopOuter));
			}

			return FString::Printf(TEXT("%s"), *Object->GetName());
		}

		return FString{TEXT("Null")};
	}

#pragma endregion

#pragma region Search
	struct FStructSearch
	{
		template<class T>
		static void SearchChildren(TArray<UScriptStruct*> Results, bool bIncludeSelf = false)
		{
			for (TObjectIterator<UScriptStruct> ScriptStructIt; ScriptStructIt; ++ScriptStructIt)
			{
				UScriptStruct* ScriptStruct = *ScriptStructIt;
				if (ScriptStruct->IsChildOf(T::StaticScript()))
				{
					Results.Emplace(ScriptStruct);
				}
			}
		}

		template<class T, class Predicate>
		static void SearchChildren(TArray<UScriptStruct*> Results, Predicate Pred, bool bIncludeSelf = false)
		{
			for (TObjectIterator<UScriptStruct> ScriptStructIt; ScriptStructIt; ++ScriptStructIt)
			{
				UScriptStruct* ScriptStruct = *ScriptStructIt;
				if (ScriptStruct->IsChildOf(T::StaticScript()) && Pred(ScriptStruct))
				{
					Results.Emplace(ScriptStruct);
				}
			}
		}
	};
#pragma endregion 
}
