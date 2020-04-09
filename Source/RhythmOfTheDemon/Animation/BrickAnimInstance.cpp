// Fill out your copyright notice in the Description page of Project Settings.

#include "BrickAnimInstance.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"

void UBrickAnimInstance::PlayAnimFromState(EBRICK_STATE state)
{
	//UE_LOG(ApplicationInitLog, Log, TEXT("* Playing anim in BrickAnimInstance"));

	switch (state)
	{
	case EBRICK_STATE::IDLE:
		
		if (m_idleStateAnim)
			Montage_Play(m_idleStateAnim);
		
		break;
	case EBRICK_STATE::PREPARATION:
		
		if (m_prepStateAnim)
			Montage_Play(m_prepStateAnim);
		
		break;
	case EBRICK_STATE::HIT:
		
		if (m_hitStateAnim)
			Montage_Play(m_hitStateAnim);
		
		break;
	case EBRICK_STATE::PASSED:

		if (m_passedStateAnim)
			Montage_Play(m_passedStateAnim);
		
		break;
	default:
		break;
	}
}

void UBrickAnimInstance::SetStateAnim(UAnimMontage* anim, EBRICK_STATE state)
{
	//UE_LOG(ApplicationInitLog, Log, TEXT("* Setting Anim in BrickAnimInstance"));

	switch (state)
	{
	case EBRICK_STATE::IDLE:
		m_idleStateAnim = anim;
		break;
	case EBRICK_STATE::PREPARATION:
		m_prepStateAnim = anim;
		break;
	case EBRICK_STATE::HIT:
		m_hitStateAnim = anim;
		break;
	case EBRICK_STATE::PASSED:
		m_passedStateAnim = anim;
		break;
	default:
		break;
	}
}
