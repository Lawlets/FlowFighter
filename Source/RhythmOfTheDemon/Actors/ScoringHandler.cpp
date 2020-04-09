// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoringHandler.h"
#include "Brick/AbstractBrick.h"
#include "RhythmOfTheDemonGameModeBase.h"
#include "Actors/Components/StreakComponnent.h"

#include "Engine.h"
#include "RhythmOfTheDemon.h"
#include "Engine/World.h"

AScoringHandler* AScoringHandler::m_instance;

AScoringHandler::AScoringHandler()
{
	//PrimaryActorTick.bCanEverTick = true;
	m_instance = this;

	m_player1Streak = this->CreateDefaultSubobject<UStreakComponnent>(TEXT("Player1 Chain Streak"));
	this->AddOwnedComponent(m_player1Streak);

	m_player2Streak = this->CreateDefaultSubobject<UStreakComponnent>(TEXT("Player2 Chain Streak"));
	this->AddOwnedComponent(m_player2Streak);
}

void AScoringHandler::BeginPlay()
{
	Super::BeginPlay();
}

//void AScoringHandler::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

void AScoringHandler::Init(EDIFFICULTIES difficulty)
{
	//UE_LOG(LogTemp, Warning, TEXT("Init scoring handler with difficulty %i"), (int)difficulty);
	m_currentDifficulty = difficulty;
}

float AScoringHandler::GainScore(AAbstractBrick* brick)
{
	float score = brick->GetScoreValue();
	float scoreGained = ComputeScore(score, brick->GetSuccessType());

	EPLAYER_TYPE targetType = brick->GetTargetType();
	if (targetType == EPLAYER_TYPE::Arm)
	{
		m_player1Streak->AddStreak();
		m_player1Score += scoreGained * m_player1Streak->GetStreak();
	}
	else if (targetType == EPLAYER_TYPE::Leg)
	{
		m_player2Streak->AddStreak();
		m_player2Score += scoreGained * m_player2Streak->GetStreak();
	}
	else
	{
		m_player1Streak->AddStreak();
		m_player2Streak->AddStreak();

		m_player1Score += scoreGained * m_player1Streak->GetStreak();
		m_player2Score += scoreGained * m_player2Streak->GetStreak();
	}

	return scoreGained;
}

ESUCCESS_TYPE AScoringHandler::ComputeSuccessType(int currentTimelinePos, AAbstractBrick* brick)
{
	int validationWindow = brick->GetActivationWindow();
	int timelineDiff = abs(currentTimelinePos - brick->GetTimelinePosition());

	//UE_LOG(ApplicationInitLog, Log, TEXT("* Current timeline is = %i"), currentTimelinePos);
	//UE_LOG(ApplicationInitLog, Log, TEXT("* perfect timeline is = %i"), brick->GetTimelinePosition());
	//UE_LOG(ApplicationInitLog, Log, TEXT("* difference with timeline is = %i"), timelineDiff);
	if (timelineDiff < validationWindow * m_perfectRequirement)
		return ESUCCESS_TYPE::Perfect;
	else if (timelineDiff < validationWindow * m_coolRequirement)
		return ESUCCESS_TYPE::Good;
	else
		return ESUCCESS_TYPE::Ok;
}

float AScoringHandler::ComputeScore(float score, ESUCCESS_TYPE success)
{
	switch (success)
	{
	case ESUCCESS_TYPE::Perfect:
		return score * m_perfectScoreMultiplier;
	case ESUCCESS_TYPE::Good:
		return score * m_coolScoreMultiplier;
	case ESUCCESS_TYPE::Ok:
		return score * m_okScoreMultiplier;
	}

	return 0;
}

float AScoringHandler::ComputeDamageScore(AAbstractBrick* brick)
{
	float scoreValue = brick->GetScoreValue();

	EPLAYER_TYPE targetType = brick->GetTargetType();
	if (targetType == EPLAYER_TYPE::Arm)
		m_player1Streak->ResetStreak();
	else if (targetType == EPLAYER_TYPE::Leg)
		m_player2Streak->ResetStreak();
	else
	{
		m_player1Streak->ResetStreak();
		m_player2Streak->ResetStreak();
	}

	float res = 0.f;

	switch (m_currentDifficulty)
	{
	case EDIFFICULTIES::Easy:
		res = scoreValue * m_easyDifficultyDamageMultiplier;
		break;

	case EDIFFICULTIES::Normal:
		res = scoreValue * m_mediumDifficultyDamageMultiplier;
		break;

	case EDIFFICULTIES::Hard:
		res = scoreValue * m_hardDifficultyDamageMultiplier;
		break;
	}

	return res;
}

float AScoringHandler::GetScoreOfPlayer(int index)
{
	if (index == 1)
		return m_player1Score;
	else if (index == 2)
		return m_player2Score;

	return 0.f;
}

void AScoringHandler::ResetScore()
{
	m_score = 0.f;

	m_player1Score = 0.f;
	m_player2Score = 0.f;

	m_player1Streak->CleanAllStreak();
	m_player2Streak->CleanAllStreak();
}