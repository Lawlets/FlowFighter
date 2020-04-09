// Fill out your copyright notice in the Description page of Project Settings.

#include "GarbageCollectorDebugger.h"
#include "UObject/ReferenceChainSearch.h"
#include "RhythmOfTheDemon.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Casts.h"

GarbageCollectorDebugger::GarbageCollectorDebugger()
{
}

GarbageCollectorDebugger::~GarbageCollectorDebugger()
{
}

int32 GetObjReferenceCount(UObject* Obj, TArray<UObject*>* OutReferredToObjects)
{
	if (!Obj || !Obj->IsValidLowLevelFast())
	{
		return -1;
	}

	TArray<UObject*> ReferredToObjects;				//req outer, ignore archetype, recursive, ignore transient
	FReferenceFinder ObjectReferenceCollector(ReferredToObjects, Obj, false, true, true, false);
	ObjectReferenceCollector.FindReferences(Obj);

	if (OutReferredToObjects)
	{
		OutReferredToObjects->Append(ReferredToObjects);
	}

	return OutReferredToObjects->Num();
}

//void GarbageCollectorDebugger::VDestroy(UObject * ToDestroy)
//{
//	if (!ToDestroy) return;
//	if (!ToDestroy->IsValidLowLevel()) return;
//	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//	//Actor?
//	AActor * AnActor = Cast<AActor>(ToDestroy);
//	if (AnActor)
//	{
//		AnActor->K2_DestroyActor();
//		ToDestroy = NULL;
//	}
//
//	//Object - BeginDestroy
//	else
//	{
//		//Begin Destroy
//		ToDestroy->ConditionalBeginDestroy();
//		ToDestroy = NULL;
//	}
//
//	//GC
//	
//	GetWorld()->ForceGarbageCollection(true);
//
//}