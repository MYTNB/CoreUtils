// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreUtils/Extensions/Math.h"

/**
 * 
 */
template <typename TValue, std::size_t SIZE>
class TInlineArray
{
public:
	TInlineArray() {}
	TInlineArray(const std::initializer_list<TValue>& InElements);
	TInlineArray(TValue&& Args...);
	
	FORCEINLINE TValue& operator[](std::size_t Index) { ensureAlways(IsValidIndex(Index)); return Array[Index]; }

	FORCEINLINE bool IsValidIndex(const std::size_t Index) const { return Index >= static_cast<std::size_t>(0) && Index < ArrayNum; }

	FORCEINLINE std::size_t Num() const { return ArrayNum; }
	FORCEINLINE std::size_t Capacity() const { return SIZE; }
	
	bool Contains(const TValue& InValue);
	bool Append(const TValue& InValue);
	bool Append(TValue&& InValue);
	bool Remove(const TValue& InValue);
	bool RemoveAll(const TValue& InValue);
	void Empty();
	template<typename Deleter>
	void Empty(Deleter Del);
	
private:
	std::size_t ArrayNum = 0;
	TValue Array[SIZE] {TValue()};
};

template <typename TValue, std::size_t SIZE>
TInlineArray<TValue, SIZE>::TInlineArray(const std::initializer_list<TValue>& InElements)
{
	ensureAlwaysMsgf(InElements.size() <= SIZE, TEXT("InElements num is out of range!"));
	ArrayNum = FGameMath::Min(InElements.size(), SIZE);
	for (int Index = 0; Index < ArrayNum; ++ Index)
	{
		Array[Index] = MoveTemp(InElements[Index]);
	}
}

template <typename TValue, std::size_t SIZE>
TInlineArray<TValue, SIZE>::TInlineArray(TValue&& Args, ...)
{
	ensureAlwaysMsgf(sizeof...(Args) <= SIZE, TEXT("InElements num is out of range!"));
	for (auto Value : Args)
	{
		Array[++ ArrayNum] = Value;
	}
}

template <typename TValue, std::size_t SIZE>
bool TInlineArray<TValue, SIZE>::Contains(const TValue& InValue)
{
	for (int Index = 0; Index < ArrayNum; ++ Index)
	{
		if (Array[Index] == InValue)
		{
			return true;
		}
	}

	return false;
}

template <typename TValue, std::size_t SIZE>
bool TInlineArray<TValue, SIZE>::Append(const TValue& InValue)
{
	if (ArrayNum >= SIZE)
	{
		return false;
	}

	Array[ArrayNum ++] = InValue;
	return true;
}

template <typename TValue, std::size_t SIZE>
bool TInlineArray<TValue, SIZE>::Append(TValue&& InValue)
{
	if (ArrayNum >= SIZE)
	{
		return false;
	}
	
	Array[ArrayNum ++] = Forward<TValue>(InValue);
	return true;
}

template <typename TValue, std::size_t SIZE>
bool TInlineArray<TValue, SIZE>::Remove(const TValue& InValue)
{
	for (int Index = 0; Index < ArrayNum; ++ Index)
	{
		if (Array[Index] == InValue)
		{
			FMemory::Memmove(Array + Index, Array + Index + 1, sizeof(TValue) * (-- ArrayNum));
			Array[ArrayNum] = TValue();
			return true;
		}
	}

	return false;
}

template <typename TValue, std::size_t SIZE>
bool TInlineArray<TValue, SIZE>::RemoveAll(const TValue& InValue)
{
	bool Result = false;
	for (int Index = 0; Index < ArrayNum; ++ Index)
	{
		if (Array[Index] == InValue)
		{
			FMemory::Memmove(Array + Index, Array + Index + 1, sizeof(TValue) * (-- ArrayNum));
			Array[ArrayNum] = TValue();
			Result = true;
		}
	}

	return Result;
}

template <typename TValue, std::size_t SIZE>
void TInlineArray<TValue, SIZE>::Empty()
{
	if constexpr (!TIsTriviallyDestructible<TValue>::Value)
	{
		for (int Index = 0; Index < ArrayNum; ++ Index)
		{
			Array[Index]->TValue::~TValue();
		}
	}
	
	ArrayNum = 0;
}

template <typename TValue, std::size_t SIZE>
template <typename Deleter>
void TInlineArray<TValue, SIZE>::Empty(Deleter Del)
{
	for (int Index = 0; Index < ArrayNum; ++ Index)
	{
		Del(Array[Index]);
	}

	FMemory::Memzero(Array, ArrayNum * sizeof(TValue));
	ArrayNum = 0;
}
