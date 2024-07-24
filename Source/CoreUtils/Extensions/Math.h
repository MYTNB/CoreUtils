// Extensions of UE Math

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct FGameMath : public FMath
{
#pragma region Number
	FORCEINLINE static bool IsInRange(int Value, int Min, int Max)
	{
		return Value >= Min && Value <= Max;
	}

	FORCEINLINE static bool IsInRange(float Value, float Min, float Max)
	{
		return Value >= Min && Value <= Max;
	}

	FORCEINLINE static bool IsInRange(double Value, double Min, double Max)
	{
		return Value >= Min && Value <= Max;
	}
	
	FORCEINLINE static float Pow2(const float Value)
	{
		return Value * Value;
	}

	FORCEINLINE static double Pow2(const double Value)
	{
		return Value * Value;
	}
	
	/** 将值限制为0~1 */
	FORCEINLINE static float Clamp01(const float Value)
	{
		return Clamp(Value, 0, 1);
	}

	/** 将值映射到0~1 */
	inline static float Normalize01(const float Value, const float MinValue, const float MaxValue)
	{
		const float Delta = MaxValue - MinValue;
		if (Delta < SMALL_NUMBER) return 0.0F;
		return (static_cast<double>(Clamp(Value, MinValue, MaxValue)) - static_cast<double>(MinValue)) / static_cast<double>(Delta);
	}

	/** 计算Atan角度值 */
	FORCEINLINE static float DegreesAtan2(const float X, const float Y)
	{
		return (180.0) / PI * atan2(X, Y);
	}

	FORCEINLINE static float SignEx(const double Value)
	{
		return Value >= 0.F ? 1.F : -1.F;
	}

	FORCEINLINE static float Lerp(const float From, const float To, const float Alpha)
	{
		return From + Clamp01(Alpha) * (To - From);
	}

	FORCEINLINE static float LerpUnClamp(const float From, const float To, const float Alpha)
	{
		return From + Alpha * (To - From);
	}
	
	FORCEINLINE static float Damper(const float From, const float To, const float DeltaSeconds, const float HalfTime)
	{
		return LerpUnClamp(From, To, 1.0F - powf(2.F, -DeltaSeconds / HalfTime));
	}
#pragma endregion Float

#pragma region Vector
	/** Normalize Vector (double) */
	inline static FVector NormalizeVector(const FVector& Vector)
	{
		const double Size = sqrt(Pow2((double)Vector.X) + Pow2((double)Vector.Y) + Pow2((double)Vector.Z));
		if (Size <= SMALL_NUMBER)
		{
			return FVector::ZeroVector;
		}
		
		const double InvSize = 1.0 / Size;
		return FVector(Vector.X * InvSize, Vector.Y * InvSize, Vector.Z * InvSize);
	}

	/** Normalize Vector2D (double) */
	inline static FVector NormalizeVector2D(const FVector& Vector)
	{
		const double Size2D = sqrt(Pow2((double)Vector.X) + Pow2((double)Vector.Y));
		if (Size2D <= SMALL_NUMBER)
		{
			return FVector::ZeroVector;
		}
		
		const double InvSize2D = 1.0 / Size2D;
		return FVector(Vector.X * InvSize2D, Vector.Y * InvSize2D, 0.0F);
	}

	/** 获取向量间的弧度值 [0~PI] */
	FORCEINLINE static float GetRadiansBetween(const FVector2D& From, const FVector2D& To)
	{
		return static_cast<float>(acos(FVector2D::DotProduct(From.GetSafeNormal(), To.GetSafeNormal())));
	}

	/** 获取向量间的角度值 [0~180] */
	FORCEINLINE static float GetDegreesBetween(const FVector2D& From, const FVector2D& To)
	{
		return RadiansToDegrees(GetRadiansBetween(From, To));
	}
	
	/** 获取向量间的弧度值 [0~PI] */
	FORCEINLINE static float GetRadiansBetween(const FVector& From, const FVector& To)
	{
		return static_cast<float>(acos(FVector::DotProduct(From.GetSafeNormal(), To.GetSafeNormal())));
	}

	/** 获取向量间的角度值 [0~180] */
	FORCEINLINE static float GetDegreesBetween(const FVector& From, const FVector& To)
	{
		return RadiansToDegrees(GetRadiansBetween(From, To));
	}

	/** 获取向量间的水平夹角 [-180~180] */
	FORCEINLINE static float GetYawBetween(const FVector& From, const FVector& To)
	{
		return RadiansToDegrees(GetRadiansYawBetween(From, To));
	}

	/** 获取向量间的水平夹角 [-PI~PI] */
	inline static float GetRadiansYawBetween(const FVector& From, const FVector& To)
	{
		const FVector From2D = NormalizeVector2D(From);
		const FVector To2D = NormalizeVector2D(To);
		const float Radians = static_cast<float>(acos(FVector::DotProduct(From2D, To2D)));
		const float RadiansSign = FMath::Sign(From2D.X * To2D.Y - From2D.Y * To2D.X);
		return RadiansSign * Radians;
	}

	/** 获取向量间的带符号夹角 [-PI ~ PI] */
	inline static float GetSignedRadiansBetween(const FVector& From, const FVector& To, const FVector& Axis)
	{
		const float Degrees = GetRadiansBetween(From, To);
		const float DegreesSign = Sign((From ^ To) | Axis);
		return DegreesSign * Degrees;
	}
	
	/** 获取向量间的带符号夹角 [-180~180] */
	inline static float GetSignedDegreesBetween(const FVector& From, const FVector& To, const FVector& Axis)
	{
		const float Degrees = GetDegreesBetween(From, To);
		const float DegreesSign = Sign((From ^ To) | Axis);
		return DegreesSign * Degrees;
	}

	FORCEINLINE static FVector Lerp(const FVector& From, const FVector& To, float Alpha)
	{
		return From + (To - From) * Clamp01(Alpha);
	}

	FORCEINLINE static FVector LerpUnClamp(const FVector& From, const FVector& To, float Alpha)
	{
		return From + (To - From) * Alpha;
	}
	
	FORCEINLINE static FVector Lerp(const FVector_NetQuantize& From, const FVector_NetQuantize& To, float Alpha)
	{
		return From + (To - From) * Clamp01(Alpha);
	}

	FORCEINLINE static FVector LerpUnClamp(const FVector_NetQuantize& From, const FVector_NetQuantize& To, float Alpha)
	{
		return From + (To - From) * Alpha;
	}

	inline static FVector2D Slerp(const FVector2D& From, const FVector2D& To, float Alpha)
	{
		const FVector2D& FromVector = From.GetSafeNormal();
		const FVector2D& ToVector = To.GetSafeNormal();
		const float Dot = FromVector | ToVector;
		const float Omega = Acos(Dot) * Clamp01(Alpha);
		const FVector2D RelativeVector = (ToVector - FromVector * Dot).GetSafeNormal();
		return (FromVector * Cos(Omega) + RelativeVector * Sin(Omega)).GetSafeNormal();
	}
	
	inline static FVector Slerp(const FVector& From, const FVector& To, float Alpha)
	{
		const FVector& FromVector = From.GetSafeNormal();
		const FVector& ToVector = To.GetSafeNormal();
		const float Dot = FromVector | ToVector;
		const float Omega = Acos(Dot) * Clamp01(Alpha);
		const FVector RelativeVector = (ToVector - FromVector * Dot).GetSafeNormal();
		return (FromVector * Cos(Omega) + RelativeVector * Sin(Omega)).GetSafeNormal();
	}

	inline static FVector2D SlerpUnClamp(const FVector2D& From, const FVector2D& To, float Alpha)
	{
		const FVector2D& FromVector = From.GetSafeNormal();
		const FVector2D& ToVector = To.GetSafeNormal();
		const float Dot = FromVector | ToVector;
		const float Omega = Acos(Dot) * Alpha;
		const FVector2D RelativeVector = (ToVector - FromVector * Dot).GetSafeNormal();
		return (FromVector * Cos(Omega) + RelativeVector * Sin(Omega)).GetSafeNormal();
	}

	inline static FVector SlerpUnClamp(const FVector& From, const FVector& To, float Alpha)
	{
		const FVector& FromVector = From.GetSafeNormal();
		const FVector& ToVector = To.GetSafeNormal();
		const float Dot = FromVector | ToVector;
		const float Omega = Acos(Dot) * Alpha;
		const FVector RelativeVector = (ToVector - FromVector * Dot).GetSafeNormal();
		return (FromVector * Cos(Omega) + RelativeVector * Sin(Omega)).GetSafeNormal();
	}
#pragma endregion Vector

#pragma region Rotation
	/** 欧拉角绝对值 */
	FORCEINLINE static FRotator AbsRotator(const FRotator& Rotator)
	{
		return FRotator(Abs(Rotator.Pitch), Abs(Rotator.Yaw), FMath::Abs(Rotator.Roll));
	}

	/** Round Rotator */
	inline static FRotator RoundToRotator(const FRotator& Rotator, const float Decimals = 100.F)
	{
		FRotator RoundedRotator;
		RoundedRotator.Roll = RoundToFloat(Rotator.Roll * Decimals) / Decimals;
		RoundedRotator.Pitch = RoundToFloat(Rotator.Pitch * Decimals) / Decimals;
		RoundedRotator.Yaw = RoundToFloat(Rotator.Yaw * Decimals) / Decimals;
		return RoundedRotator;
	}
	
	/** 欧拉角之间的旋转角 */
	FORCEINLINE static FRotator GetRotatorBetween(const FRotator& From, const FRotator& To)
	{
		return To - From;
	}

	/** 欧拉角之间的旋转四元数 */
	FORCEINLINE static FQuat GetRotationBetween(const FRotator& From, const FRotator& To)
	{
		return (To - From).Quaternion();
	}
	
	FORCEINLINE static float GetRadiansBetween(const FRotator& From, const FRotator& To)
	{
		return GetRadiansBetween(From.Quaternion().GetForwardVector(), To.Quaternion().GetForwardVector());
	}

	FORCEINLINE static float GetRadiansBetween(const FQuat& From, const FQuat& To)
	{
		return GetRadiansBetween(From.GetForwardVector(), To.GetForwardVector());
	}

	FORCEINLINE static float GetDegreesBetween(const FRotator& From, const FRotator& To)
	{
		return GetDegreesBetween(From.Quaternion().GetForwardVector(), To.Quaternion().GetForwardVector());
	}

	FORCEINLINE static FRotator LookRotator(const FVector& ForwardVector, const FVector& UpAxis)
	{
		return LookRotation(ForwardVector, UpAxis).Rotator();
	}
	
	FORCEINLINE static FQuat LookRotation(const FVector& ForwardVector, const FVector& UpAxis=FVector::UpVector)
	{
		return FRotationMatrix::MakeFromXZ(ForwardVector, UpAxis).ToQuat();
	}

	inline static FQuat GetPanelRotationBetween(const FVector& From, const FVector& To, const FVector& PanelNormal)
	{
		FVector FromVector = FVector::VectorPlaneProject(From, PanelNormal);
		FVector ToVector = FVector::VectorPlaneProject(To, PanelNormal);

		if (FromVector.IsNearlyZero() || ToVector.IsNearlyZero())
		{
			return FQuat::Identity;
		}
	
		FromVector = FromVector.GetUnsafeNormal();
		ToVector = ToVector.GetUnsafeNormal();
	
		const float Angle = acos(Clamp(FVector::DotProduct(FromVector, ToVector), 0.0F, 1.0F));
		const float AngleSign = Sign(FVector::DotProduct(FromVector ^ ToVector, PanelNormal));
		return FQuat(PanelNormal, AngleSign * Angle);
	}

	inline static FQuat RotateTowards(const FQuat& From, const FQuat& To, const float RotateAngle)
	{
		const float Angle = From.AngularDistance(To);
		if (Angle <= KINDA_SMALL_NUMBER) return To;
		return FQuat::Slerp(From, To, FMath::Min(1.0f, RotateAngle / Angle));
	}
#pragma endregion Rotation
};
