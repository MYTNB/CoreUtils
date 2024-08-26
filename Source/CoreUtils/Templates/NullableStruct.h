// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template <typename TStruct>
struct TNullableStruct
{
	TNullableStruct() : bHasValue(false), Value(TStruct()) {}
	TNullableStruct(const TStruct& InValue) : bHasValue(true), Value(InValue) {}
	TNullableStruct(TStruct&& InValue) : bHasValue(true), Value(MoveTempIfPossible(InValue)) {}

	FORCEINLINE TStruct* operator->() { check(HasValue()); return &Value; }
	FORCEINLINE const TStruct* operator->() const { check(HasValue()); return &Value; }
	FORCEINLINE TStruct& operator*() { return Value; }
	
	FORCEINLINE explicit operator bool() const { return bHasValue; }
	FORCEINLINE bool HasValue() const { return bHasValue; }
	
	void Set(const TStruct& InValue) { bHasValue = true; Value = InValue; }
	void Set(TStruct&& InValue) { bHasValue = true; Value = MoveTempIfPossible(InValue); }
	void Reset() { bHasValue = false; Value = TStruct(); }
	
private:
	uint8 bHasValue:1;
	TStruct Value;
};
