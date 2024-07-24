// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#pragma region Math

#define DOT_PRODUCT_0   (1.F)
#define DOT_PRODUCT_15  (0.966F)
#define DOT_PRODUCT_30  (0.866F)
#define DOT_PRODUCT_45  (0.707F)
#define DOT_PRODUCT_60  (0.5F)
#define DOT_PRODUCT_75  (0.259F)
#define DOT_PRODUCT_90  (0.F)
#define DOT_PRODUCT_105 (-0.259F)
#define DOT_PRODUCT_120 (-0.5F)
#define DOT_PRODUCT_135 (-0.707F)
#define DOT_PRODUCT_150 (-0.866F)
#define DOT_PRODUCT_165 (-0.966F)
#define DOT_PRODUCT_180 (-1.F)

#pragma endregion

#pragma region Printer

// LOG 帧数
#define LOG_FRAME_COUNTER GFrameCounter
// LOG 时间戳
#define LOG_TIME_STAMP (*FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S.%s")))
// LOG 实例名
#define LOG_OBJECT_NAME(Object) (*CoreUtils::ToLogString(Object))

// 通用打印日志宏
#define PRINT_LOG(CategoryName, Verbosity, Printer, Format, ...) \
	UE_LOG(CategoryName, Verbosity, TEXT("[%s F: %d] %s -- %s"), \
		LOG_TIME_STAMP, LOG_FRAME_COUNTER, Printer, *FString::Printf(Format, ##__VA_ARGS__))
// 条件打印日志宏
#define PRINT_CLOG(Condition, CategoryName, Verbosity, Printer, Format, ...) \
	UE_CLOG(Condition, CategoryName, Verbosity, TEXT("[%s F: %d] %s -- %s"), \
		LOG_TIME_STAMP, LOG_FRAME_COUNTER, Printer, *FString::Printf(Format, ##__VA_ARGS__))
// UObject打印日志宏
#define PRINT_OBJ_LOG(CategoryName, Verbosity, Object, Format, ...) \
	PRINT_LOG(CategoryName, Verbosity, LOG_OBJECT_NAME(Object), Format, ##__VA_ARGS__)
// UObject条件打印日志宏
#define PRINT_OBJ_CLOG(Condition, CategoryName, Verbosity, Object, Format, ...) \
	PRINT_CLOG(Condition, CategoryName, Verbosity, LOG_OBJECT_NAME(Object), Format, ##__VA_ARGS__)

#pragma endregion

#pragma region LOG

// Info(不会输出到Console)
#define LOG_INFO(CategoryName, Format, ...) \
	PRINT_OBJ_LOG(CategoryName, Log, this, Format, ##__VA_ARGS__)
// Debug
#define LOG_DEBUG(CategoryName, Format, ...) \
	PRINT_OBJ_LOG(CategoryName, Display, this, Format, ##__VA_ARGS__)
// Warning
#define LOG_WARNING(CategoryName, Format, ...) \
	PRINT_OBJ_LOG(CategoryName, Warning, this, Format, ##__VA_ARGS__)
// Error
#define LOG_ERROR(CategoryName, Format, ...) \
	PRINT_OBJ_LOG(CategoryName, Error, this, Format, ##__VA_ARGS__)
// EX Info
#define LOG_INFO_EX(Printer, CategoryName, Format, ...) \
	PRINT_LOG(CategoryName, Log, Printer, Format, ##__VA_ARGS__)
// EX Debug
#define LOG_DEBUG_EX(Printer, CategoryName, Format, ...) \
	PRINT_LOG(CategoryName, Display, Printer, Format, ##__VA_ARGS__)
// EX Warning
#define LOG_WARNING_EX(Printer, CategoryName, Format, ...) \
	PRINT_LOG(CategoryName, Warning, Printer, Format, ##__VA_ARGS__)
// EX Error
#define LOG_ERROR_EX(Printer, CategoryName, Format, ...) \
	PRINT_LOG(CategoryName, Error, Printer, Format, ##__VA_ARGS__)

#pragma endregion

#pragma region CLOG

// C Info
#define CLOG_INFO(Condition, CategoryName, Format, ...) \
	PRINT_OBJ_CLOG(Condition, CategoryName, Log, this, Format, ##__VA_ARGS__)
// C Debug
#define CLOG_DEBUG(Condition, CategoryName, Format, ...) \
	PRINT_OBJ_CLOG(Condition, CategoryName, Display, this, Format, ##__VA_ARGS__)
// C Warning
#define CLOG_WARNING(Condition, CategoryName, Format, ...) \
	PRINT_OBJ_CLOG(Condition, CategoryName, Warning, this, Format, ##__VA_ARGS__)
// C Error
#define CLOG_ERROR(Condition, CategoryName, Format, ...) \
	PRINT_OBJ_CLOG(Condition, CategoryName, Error, this, Format, ##__VA_ARGS__)
// Assert
#define LOG_ASSERT(Condition, CategoryName, Format, ...) \
	{ \
		if (!(Condition)) { \
			PRINT_OBJ_LOG(CategoryName, Fatal, this, Format, ##__VA_ARGS__); \
		} \
	}
// EX Assert
#define LOG_ASSERT_EX(Condition, Printer, CategoryName, Format, ...) \
	{ \
		if (!(Condition)) { \
			PRINT_LOG(CategoryName, Fatal, Printer, Format, ##__VA_ARGS__); \
		} \
	}

#pragma endregion

#pragma region STRUCT

#define USTRUCT_BODY(Type) \
	virtual UScriptStruct* GetScriptStruct() const { return StaticStruct(); } \
	template<class T> FORCEINLINE bool IsA() const \
	{ \
		static_assert(TIsDerivedFrom<T, Type>::IsDerived); \
		return GetScriptStruct()->IsChildOf(T::StaticStruct()); \
	} \
	/** Safe Cast*/ \
	template<class T> FORCEINLINE T* CastTo() \
	{ \
		return IsA<T>() ? (T*)this : nullptr; \
	} \
	/** Safe Cast*/ \
	template<class T> FORCEINLINE const T* CastTo() const \
	{ \
		return IsA<T>() ? (const T*)this : nullptr; \
	} \
	/** Unsafe Cast*/ \
		template<class T> FORCEINLINE T* CastToFast() \
	{ \
		return (T*)this; \
	} \
	/** Unsafe Cast*/ \
	template<class T> FORCEINLINE const T* CastToFast() const \
	{ \
		return (const T*)this; \
	}

#define USTRUCT_BODY_DEFAULT(Type) \
	virtual ~Type() = default; \
	USTRUCT_BODY(Type)

#define USTRUCT_BODY_OVERRIDE(Type) \
	virtual ~Type() override = default; \
	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }

#pragma endregion

#pragma region FUNCTION

#define FUNC_NOT_IMPL \
	{\
		ensureAlwaysMsgf(false, TEXT("Function: %s not implementated!"), ANSI_TO_TCHAR(__FUNCTION__)); \
	}

#define FUNC_NOT_IMPL_RET(Value) \
	{\
		ensureAlwaysMsgf(false, TEXT("Function: %s not implementated!"), ANSI_TO_TCHAR(__FUNCTION__)); \
		return Value;\
	}

#pragma endregion
