// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct COREUTILS_API FUInt128BitSet
{
	uint64 High = 0lu;
	uint64 Low = 0lu;

	explicit operator bool() const
	{
		return (High | Low) != 0llu;
	}
	
	void Zero()
	{
		High = Low = 0llu;
	}
	
	// Index in [0, 128)
	bool IsBitSet(int32 Index) const
	{
		if (Index >= 128)
		{
			return false;
		}

		uint64 Value = Low;
		if (Index >= 64)
		{
			Value = High;
			Index -= 64;
		}
		
		return (Value & (1llu << Index)) != 0llu;
	}

	void BitSet(int32 Index)
	{
		if (Index >= 128)
		{
			return;
		}

		uint64& Value = Low;
		if (Index >= 64)
		{
			Value = High;
			Index -= 64;
		}
		
		Value |= (1llu << Index);
	}

	void BitReset(int32 Index)
	{
		if (Index >= 128)
		{
			return;
		}

		uint64& Value = Low;
		if (Index >= 64)
		{
			Value = High;
			Index -= 64;
		}
		
		Value &= ~(1llu << Index);
	}
};

struct COREUTILS_API FUInt256BitSet
{
	uint64 Numbers[4];

	FUInt256BitSet()
	{
		Zero();
	}
	
	explicit operator bool() const
	{
		return (Numbers[0] | Numbers[1] | Numbers[2] | Numbers[3]) != 0llu;
	}
	
	void Zero()
	{
		std::memset(Numbers, 0, sizeof(Numbers));
	}
	
	// Index in [0, 128)
	bool IsBitSet(int32 Index) const
	{
		const uint64 P = Index / 64llu;
		const uint64 Q = Index % 64llu;
		if (P >= 4llu)
		{
			return false;
		}
		
		return (Numbers[P] & (1llu << Q)) != 0llu;
	}

	void BitSet(int32 Index)
	{
		const uint64 P = Index / 64llu;
		const uint64 Q = Index % 64llu;
		if (P >= 4llu)
		{
			return;
		}
		
		Numbers[P] |= (1llu << Q);
	}

	void BitReset(int32 Index)
	{
		const uint64 P = Index / 64llu;
		const uint64 Q = Index % 64llu;
		if (P >= 4llu)
		{
			return;
		}
		
		Numbers[P] &= ~(1llu << Q);
	}
};
