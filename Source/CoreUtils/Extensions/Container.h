// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * UE 容器类拓展
 */
struct FContainer
{
#pragma region Array
	// Unique 数组
	template<class T>
	static void Unique(TArray<T>& Array, bool bAllowShrinking = true)
	{
		int Num = 0;
		int Last = Array.Num();
		for (int i = 1; i < Array.Num(); ++ i)
		{
			for (int Index = 0; Index < i; ++ Index)
			{
				if (Array[Index] == Array[i])
				{
					Array.Swap(++ Num, -- Last);
					break;
				}
			}
		}

		if (Last < Array.Num())
		{
			Array.RemoveAt(Last, Array.Num() - Last, bAllowShrinking);
		}
	}

	// Unique 数组 (可以提供自定义Compare函数)
	template<class T, class Predicate>
	static void Unique(TArray<T>& Array, Predicate Compare, bool bAllowShrinking = true)
	{
		int Num = 0;
		int Last = Array.Num();
		for (int i = 1; i < Array.Num(); ++ i)
		{
			for (int Index = 0; Index < i; ++ Index)
			{
				if (Compare(Array[Index], Array[i]))
				{
					Array.Swap(++ Num, -- Last);
					break;
				}
			}
		}

		if (Last < Array.Num())
		{
			Array.RemoveAt(Last, Array.Num() - Last, bAllowShrinking);
		}
	}
#pragma endregion

#pragma region Set
	/**
	 * is S1 & S2 not Empty ?
	 */
	template<typename T>
	static bool IsIntersect(const TSet<T>& S1, const TSet<T>& S2)
	{
		if (S1.Num() > S2.Num())
		{
			for (auto& E : S2)
			{
				if (S1.Contains(E))
					return true;
			}
		}
		else
		{
			for (auto& E : S1)
			{
				if (S2.Contains(E))
					return true;
			}
		}
		
		return false;
	}

	/**
	 * S1 - S2
	 */
	template<typename T>
	static void ExceptWith(TSet<T>& S1, const TSet<T>& S2)
	{
		// S1 < S2 时才会遍历 S1
		if (S1.Num() > S2.Num())
		{
			for (const T& Element : S2)
			{
				S1.Remove(Element);
			}
		}
		else
		{
			for (auto Iter = S1.CreateIterator(); Iter; ++ Iter)
			{
				if (S2.Contains(*Iter))
				{
					Iter.RemoveCurrent();
				}
			}
		}
	}

	/**
	 * S1 | S2
	 */
	template<typename T>
	static void UniqueWith(TSet<T>& S1, const TSet<T>& S2)
	{
		S1.Append(S2);
	}
#pragma endregion

#pragma region Map
	// 
	template<class TKey, class TValue>
	static TMap<TKey, TValue> MakeMap(const TArray<TKey>& Keys, const TArray<TValue>& Values)
	{
		const int MapNum = FMath::Min(Keys.Num(), Values.Num());

		TMap<TKey, TValue> Map;
		for (int i = 0; i < MapNum; ++ i)
		{
			Map.Emplace(Keys[i], Values[i]);
		}

		for (int i = MapNum; i < Keys.Num(); ++ i)
		{
			Map.Emplace(Keys[i], TValue());	
		}
		
		return Map;
	}

	template<class TKey, class TValue>
	static TValue* FindOrDefault(const TMap<TKey, TValue>& Map, const TKey& Key, TValue* Default = nullptr)
	{
		if (const TValue* ValuePtr = Map.Find(Key))
		{
			return ValuePtr;
		}

		return Default;
	}
	
	template<class TKey, class TValue>
	static TValue FindRefOrDefault(const TMap<TKey, TValue>& Map, const TKey& Key, const TValue& Default = TValue())
	{
		if (const TValue* ValuePtr = Map.Find(Key))
		{
			return *ValuePtr;
		}

		return Default;
	}
#pragma endregion
};
