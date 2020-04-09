#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
static class RHYTHMOFTHEDEMON_API GarbageCollectorDebugger
{
public:
	GarbageCollectorDebugger();
	~GarbageCollectorDebugger();

	static int32 GetObjReferenceCount(UObject* Obj, TArray<UObject*>* OutReferredToObjects = nullptr);

	//static void VDestroy(UObject * ToDestroy);

};
