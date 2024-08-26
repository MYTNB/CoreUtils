// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template<
	typename KeyType1,
	typename KeyType2,
	typename ValueType,
	typename SetAllocator = FDefaultSetAllocator,
	typename KeyFuncs1 = TDefaultMapHashableKeyFuncs<KeyType1, TDefaultMapHashableKeyFuncs<KeyType2, ValueType, false>, false>,
	typename KeyFuncs2 = TDefaultMapHashableKeyFuncs<KeyType2, ValueType, false>
>
class TTwoKeyMap : TMap<KeyType1, TMap<KeyType2, ValueType, SetAllocator, KeyFuncs2>, SetAllocator, KeyFuncs1>
{
	typedef TMap<KeyType1, TMap<KeyType2, ValueType, SetAllocator, KeyFuncs2>, SetAllocator, KeyFuncs1> Super;
	typedef TMap<KeyType2, ValueType, SetAllocator, KeyFuncs2> InnerValueMap;
	typedef typename Super::KeyConstPointerType KeyConstPointerType1;
	typedef typename InnerValueMap::KeyConstPointerType KeyConstPointerType2;
	
public:

	/**
	 * Returns true if the map is empty and contains no elements. 
	 *
	 * @returns True if the map is empty.
	 * @see Num
	 */
	bool IsEmpty() const
	{
		for (auto& Element : Super::Pairs)
		{
			if (!Element.Value.IsEmpty())
			{
				return false;
			}
		}

		return true;
	}
	
	// Since we implement an overloaded Num() function in TMultiMap, we need to reimplement TMapBase::Num to make it visible.
	FORCEINLINE int32 Num() const
	{
		int32 Result = 0;
		for (auto& Element : Super::Pairs)
		{
			Result += Element.Value.Num();
		}

		return Result;
	}
	
	/**
	 * Set the value associated with a key.
	 *
	 * @param InKey1 The first key to associate the value with.
	 * @param InKey2 The second key to associate the value with.
	 * @param InValue The value to associate with the key.
	 * @return A reference to the value as stored in the map. The reference is only valid until the next change to any key in the map.
	 */
	FORCEINLINE ValueType& Add(const KeyType1&  InKey1, const KeyType2& InKey2, const ValueType&  InValue) { return Emplace(InKey1, InKey2, InValue); }
	FORCEINLINE ValueType& Add(const KeyType1&  InKey1, const KeyType2& InKey2, ValueType&&  InValue) { return Emplace(InKey1, InKey2, MoveTempIfPossible(InValue)); }
	FORCEINLINE ValueType& Add(KeyType1&&  InKey1, KeyType2&& InKey2, const ValueType&  InValue) { return Emplace(MoveTempIfPossible(InKey1), MoveTempIfPossible(InKey2), InValue); }
	FORCEINLINE ValueType& Add(KeyType1&&  InKey1, KeyType2&& InKey2, ValueType&& InValue) { return Emplace(MoveTempIfPossible(InKey1), MoveTempIfPossible(InKey2), MoveTempIfPossible(InValue)); }
	
	/**
	 * Sets the value associated with a key.
	 *
	 * @param InKey1 The key to associate the value with.
	 * @param InKey2 The key to associate the value with.
	 * @param InValue The value to associate with the key.
	 * @return A reference to the value as stored in the map. The reference is only valid until the next change to any key in the map.	 */
	template <typename InitKeyType1 = KeyType1, typename InitKeyType2 = KeyType2, typename InitValueType = ValueType>
	ValueType& Emplace(InitKeyType1&& InKey1, InitKeyType2&& InKey2, InitValueType&& InValue)
	{
		auto& InnerMap = Super::FindOrAdd(InKey1, InnerValueMap());
		return InnerMap.Emplace(InKey2, InValue);
	}
	
	/**
	 * Find the value associated with a specified key pair.
	 *
	 * @param Key1 The key to search for first.
	 * @param Key2 The key to search for second.
	 * @return A pointer to the value associated with the specified key pair, or nullptr if the key pair isn't contained in this map.  The pointer
	 *			is only valid until the next change to any key pair in the map.
	 */
	FORCEINLINE ValueType* Find(KeyConstPointerType1 Key1, KeyConstPointerType2 Key2)
	{
		if (auto* Pair = Super::Find(Key1))
		{
			return Pair->Value.Find(Key2);
		}

		return nullptr;
	}
	FORCEINLINE const ValueType* Find(KeyConstPointerType1 Key1, KeyConstPointerType2 Key2) const
	{
		return const_cast<TMapBase*>(this)->Find(Key1, Key2);
	}

	/**
	 * Remove all value associations for a key.
	 *
	 * @param InKey1 The first key to remove associated values for.
	 * @param InKey2 The second key to remove associated values for.
	 * @param bAllowShrinking
	 * @return The number of values that were associated with the key.
	 */
	FORCEINLINE int32 Remove(KeyConstPointerType1 InKey1, KeyConstPointerType2 InKey2, bool bAllowShrinking = false)
	{
		if (auto* Pair = Super::Find(InKey1))
		{
			const int32 Result = Pair->Value.Remove(InKey2);
			if (bAllowShrinking && Pair->Value.IsEmpty())
			{
				Super::Pairs.Remove(InKey1);
			}

			return Result;
		}

		return INDEX_NONE;
	}
};
