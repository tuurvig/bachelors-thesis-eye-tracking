// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// 2016 VRg, inc.

#pragma once
#include "../Core/Public/Modules/ModuleManager.h"
#include "IHeadMountedDisplayModule.h"

#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION == 4
#if ENGINE_MINOR_VERSION == 27
#define UVER 427
#endif
#if ENGINE_MINOR_VERSION == 26
#define UVER 426
#endif
#if ENGINE_MINOR_VERSION == 25
#define UVER 425
#endif
#if ENGINE_MINOR_VERSION == 24
#define UVER 424
#endif
#if ENGINE_MINOR_VERSION == 23
#define UVER 423
#endif
#if ENGINE_MINOR_VERSION == 22
#define UVER 422
#endif
#if ENGINE_MINOR_VERSION == 21
#define UVER 421
#endif
#if ENGINE_MINOR_VERSION == 20
#define UVER 420
#endif
#if ENGINE_MINOR_VERSION == 19
#define UVER 419
#endif
#if ENGINE_MINOR_VERSION == 18
#define UVER 418
#endif
#if ENGINE_MINOR_VERSION == 17
#define UVER 417
#endif
#if ENGINE_MINOR_VERSION == 16
#define UVER 416
#endif
#endif

/**
 * The public interface to this module.  In most cases, this interface is only public to sibling modules
 * within this plugin.
 */
class IVRgPlugin : public IHeadMountedDisplayModule {
    FString GetModuleKeyName() const {
        return FString(TEXT("vrgHMD"));
    }

public:
    /**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */

    static inline IVRgPlugin& Get() {
        return FModuleManager::LoadModuleChecked<IVRgPlugin>("vrgHMD");
    }

    static inline bool IsAvailable() {
        return FModuleManager::Get().IsModuleLoaded("vrgHMD");
    }

private:
};
