// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Common/CoreMacros.h"
#include "Common/CoreTypes.h"
#include "Common/CoreUtils.h"
#include "Extensions/Container.h"
#include "Extensions/Math.h"
#include "Templates/InlineArray.h"
#include "Templates/TwoKeyMap.h"
#include "Templates/NullableStruct.h"

class FCoreUtilsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
